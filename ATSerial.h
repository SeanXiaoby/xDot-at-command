#ifndef ATSERIAL_H
#define ATSERIAL_H

#include "mbed.h"

namespace mts
{

/** This class wraps provides a buffered wrapper to the mbed::UnbufferedSerial
 * class.  Looks for an escape sequence within received byte stream.
 */
class ATSerial : private mbed::NonCopyable<ATSerial>
{
public:
    /** Creates a new ATSerial object that can be used to talk to an mbed serial port
    * through internal SW buffers.  Providing RTS and CTS will enable flow control.
    *
    * @param txd the transmit data pin on the desired mbed Serial interface.
    * @param rxd the receive data pin on the desired mbed Serial interface.
    * @param rts the request-to-send pin on the desired mbed Serial interface.
    * @param cts the clear-to-send pin on the desired mbed Serial interface.
    * @param baud The initial baudrate
    */
    ATSerial(PinName txd, PinName rxd, PinName rts = NC, PinName cts = NC,
        int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);

    /** Destructs an ATSerial object and frees all related resources, including
    * internal buffers.
    */
    virtual ~ATSerial();

    /** This method is used to the set the baud rate of the serial port.
    *
    * @param baudrate the baudrate in bps as an int. The default is 9600 bps.
    */
    void baud(int baudrate);

    /** This method sets the transmission format used by the serial port.
    *
    * @param bits the number of bits in a word (5-8; default = 8)
    * @param parity the parity used (SerialBase::None, SerialBase::Odd, SerialBase::Even,
    * SerialBase::Forced1, SerialBase::Forced0; default = SerialBase::None)
    * @param stop the number of stop bits (1 or 2; default = 1)
    */
    void format(int bits=8, mbed::SerialBase::Parity parity=mbed::SerialBase::None, int stop_bits=1);

    /** Check if bytes are available to read.
     * @return True if receive buffer is not empty.
     */
    bool readable();

    /** Check if bytes can be written.
     * @return True if transmit buffer is not full.
     */
    bool writeable();

    /** Clear the receive buffer. */
    void rxClear();

    /** Clear the transmist buffer. */
    void txClear();

    /** Check for escape sequence detected on serial input
     * @return true if escape sequence was seen
     */
    bool escaped();

    /** Set escape character. */
    void escapeChar(char esc);

    /** Get the escape character. */
    char escapeChar();

    /** Clear escaped state. */
    void clearEscaped();

    /** Read a byte from receive buffer
     * @param[out] c Storage for read character 
     * @return True if a character was read
     */
    bool read(char& c);

    /** Read bytes from the receive buffer.
     * @param[out] buffer Allocated memory to store read bytes
     * @param length Size of buffer in bytes
     * @return Number of bytes read
     */
    int read(char *buffer, size_t length);

    /** Write bytes from to transmit buffer.
     * @param buffer Bytes to write
     * @param length Size of buffer in bytes
     * @return Number of bytes written
     */
    int write(const char *buffer, size_t length);

    /** Write formatted string to transmit buffer.
     * @param format Format string
     * @param ... Items to format
     * @return Number of bytes written
     */
    int writef(const char* format, ... );

protected:
    mbed::UnbufferedSerial _serial; // Using unbuffered serial so reads can be done in event handler

    // Receive buffer
    mbed::CircularBuffer<char, MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE> _rxbuf;
    // Transmit buffer
    mbed::CircularBuffer<char, MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE> _txbuf;
    bool _tx_irq_enabled;   // Flag indicating transmit IRQ is enabled

    Timer _timer;           // Inter-byte receive timer
    std::chrono::milliseconds _last_time;   // Last time a byte was received
    int _esc_cnt;           // Number of escape characters received
    char _esc_ch;           // Escape character
    bool _escaped;          // True if escape sequence has been received
    void handleRead();      // Method for handling data to be read
    void handleWrite();     // Method for handling data writes
    void startWrite();      // Method for starting data writes
    PlatformMutex _mutex;   // Lock for API accesses
    bool _flow;             // Flag indicates flow control is enabled
    DigitalOut _rts;        // Request to send signal
    InterruptIn _cts;       // Clear to send signal
    size_t _hwm;            // RX buffer high water mark for setting RTS to stop
    size_t _lwm;            // RX buffer low water mark for setting RTS to start
};

}

#endif /* ATSERIAL_H */
