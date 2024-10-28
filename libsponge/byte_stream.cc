#include "byte_stream.hh"
#include <cstddef>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity): _cap(capacity) { }

size_t ByteStream::write(const string &data) {
    size_t input = 0;
    if (input_ended()) {
        return 0;
    } 
    for (auto c : data) {
        if (_size >= _cap) break;
        _dq.push_back(c);  
        ++_size, ++input;
        ++_write_count;
    }
    return input;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string output;
    for (size_t i = 0; i < (_size < len ? _size : len); ++i) {
        output.push_back(_dq.at(i));
    }
    return output;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t i = _size < len ? _size : len;
    while (i--) {
        _dq.pop_front();
        --_size;
        ++_read_count;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string output;
    size_t i = _size < len ? _size : len;
    while (i--) {
        output.push_back(_dq.front());
        _dq.pop_front();
        --_size;
        ++_read_count;
    }
    return output;
}

void ByteStream::end_input() { _input_ended = true; }

bool ByteStream::input_ended() const { return _input_ended; }

size_t ByteStream::buffer_size() const { return _size; }

bool ByteStream::buffer_empty() const { return _size == 0; }

bool ByteStream::eof() const { return _input_ended && _size == 0; }

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _cap - _size; }
