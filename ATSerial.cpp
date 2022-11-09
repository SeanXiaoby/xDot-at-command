
#include "ATSerial.h"
#include "MTSLog.h"
#include "Utils.h"

using namespace mts;


ATSerial::ATSerial(PinName txd, PinName rxd, PinName rts, PinName cts, int baud)
    : _serial(txd, rxd, baud),
    _tx_irq_enabled(false),
    _last_time(0),
    _esc_cnt(0),
    _esc_ch('+'),
    _escaped(false),
    _flow(false),       // Flow control disabled by default
    _rts(rts),
    _cts(cts)
{
    if (rts != NC && cts != NC) {   // RTS and CTS must both be provided for flow control
        _flow = true;
        _rts = 0;   // Start with receive enabled
        _cts.fall(callback(this, &ATSerial::startWrite));   // Restart writes when able to send
        _hwm = mts_max(MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE - 10, MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE * 0.85);
        _lwm = MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE * 0.3;
    }
    _timer.start();
    _serial.attach(callback(this, &ATSerial::handleRead), SerialBase::RxIrq);
}

ATSerial::~ATSerial()
{
}

void ATSerial::baud(int baudrate) {
    _mutex.lock();
    _serial.baud(baudrate);
    _mutex.unlock();
}

void ATSerial::format(int bits, SerialBase::Parity parity, int stop_bits) {
    _mutex.lock();
    _serial.format(bits, parity, stop_bits);
    _mutex.unlock();
}
 
bool ATSerial::readable() {
    return !_rxbuf.empty();
}

bool ATSerial::writeable() {
    return !_txbuf.full();
}

void ATSerial::rxClear() {
    _mutex.lock();
    _rxbuf.reset();
    if (_flow) {
        _rts = 0;   // Allow receiving because receive buffer is now empty
    }
    _mutex.unlock();
}

void ATSerial::txClear() {
    _mutex.lock();
    _txbuf.reset();
    _mutex.unlock();
}

bool ATSerial::escaped() {
    _mutex.lock();
    std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time());
    std::chrono::milliseconds elapsed_ms = now - _last_time;

    // Have we seen three esc chars and 1 sec end guard has passed
    if (_escaped || (_esc_cnt == 3 && (elapsed_ms > 1s))) {
        _escaped = true;

    // Have we seen a couple esc chars but nothing in 500 ms
    } else if (_esc_cnt > 0 && _esc_cnt != 3 && elapsed_ms > 500ms) {
        // Write seen esc chars
        while (_esc_cnt) {
            _rxbuf.push(_esc_ch);
            _esc_cnt--;
        }
        _escaped = false;
    }
    _mutex.unlock();

    return _escaped;
}

void ATSerial::clearEscaped() {
    _mutex.lock();
    _esc_cnt = 0;
    _escaped = false;
    _mutex.unlock();
}

bool ATSerial::read(char& c) {
    return read(&c, 1) == 1;
}

int ATSerial::write(const char *buffer, size_t length) {
    _mutex.lock();
    size_t i = 0;
    while (i < length) {
        if (_txbuf.full()) {
            do {
                _mutex.unlock();
                thread_sleep_for(1);
                _mutex.lock();
            } while (_txbuf.full());
        }
        while (i < length && !_txbuf.full())
        {
            _txbuf.push(buffer[i]);
            i++;
        }
        startWrite();   // Start writing data in tx buffer
    }
    _mutex.unlock();
    return i;
}
    
int ATSerial::writef(const char* format, ... ) {
    char buff[256];

    va_list ap;
    va_start(ap, format);
    int size = vsnprintf(buff, 256, format, ap);
    int n = write(buff, size);
    va_end(ap);

    return n;
}

int ATSerial::read(char *buffer, size_t length) {
    _mutex.lock();
    size_t r = 0;
    while (r < length) {
        if (_rxbuf.pop(buffer[r])) {
            r++;
        } else {
            break;
        }
    }
    if (_flow && _rts && _rxbuf.size() <= _lwm) {
        _rts = 0;   // RX buffer has room, clear RTS to continue receiving
    }
    _mutex.unlock();
    return r;
}

void ATSerial::startWrite()
{
    core_util_critical_section_enter();
    if (!_tx_irq_enabled) {
        // only write to hardware in one place
        handleWrite();
        if (!_txbuf.empty()) {
            _serial.attach(callback(this, &ATSerial::handleWrite), SerialBase::TxIrq);
            _tx_irq_enabled = true;
        }
    }
    core_util_critical_section_exit();
}

void ATSerial::handleWrite()
{ 
    char c;
    while (_serial.writeable()) {
        if (_flow && _cts) {
            break;  // Exit write loop when CTS is set, will resume when it is cleared
        }

        if (_txbuf.pop(c)) {
            _serial.write(&c, 1);
        } else {
            break;
        }
    }

    // Detach TX IRQ if there's no more data to write or CTS is set
    if (_tx_irq_enabled && (_txbuf.empty() || (_flow && _cts))) {
        _serial.attach(NULL, SerialBase::TxIrq);
        _tx_irq_enabled = false;
    }
}


void ATSerial::handleRead()
{
    char byte;
    if (_serial.read(&byte, 1) < 1) { return; }

    std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>(_timer.elapsed_time());
    std::chrono::milliseconds elapsed_ms = now - _last_time;
    _last_time = now;
    
    // Have we seen 3 esc chars but this char is before 1 sec end guard time
    if (_esc_cnt == 3 && (elapsed_ms < std::chrono::seconds(1))) {
        // Write the three chars we held back
        while (_esc_cnt) {
            _rxbuf.push(_esc_ch);
            _esc_cnt--;
        }
    } else if (byte == _esc_ch) {
        // Has 1 second passed before last char
        if (elapsed_ms > std::chrono::seconds(1)) {
            _esc_cnt = 1;
        // Is this second or third esc char
        } else if (_esc_cnt > 0 && _esc_cnt < 3) {
            _esc_cnt++;
        }
    } else if (_esc_cnt > 0) {
        // Write any esc chars held back
        while (_esc_cnt) {
            _rxbuf.push(_esc_ch);
            _esc_cnt--;
        }
    }

    if(_esc_cnt == 0) {
        if (_flow && !_rts && _rxbuf.size() >= _hwm) {
            _rts = 1;   // RX buffer too full, set RTS to stop receiving
                        // Data will still be received until the buffer is full
        }

        if (_rxbuf.full()) {
            // Overflow, drop byte
        } else {
            _rxbuf.push(byte);
        }
    }

}

void ATSerial::escapeChar(char esc) {
    _esc_ch = esc;
}

char ATSerial::escapeChar() {
    return _esc_ch;
}

