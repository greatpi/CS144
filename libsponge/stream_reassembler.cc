#include "stream_reassembler.hh"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <sys/types.h>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), _stream(capacity, {0, false}) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    uint64_t st = max(index, _idx); // 防止overlap
    uint64_t ed = min(index + data.size(), min(_lastidx, _idx + _capacity - _output.buffer_size()));

    if (eof) _lastidx = min(_lastidx, index + data.size());

    for (uint64_t i = st, j = st - index; i < ed; ++i, ++j) {
        auto &[c, b] = _stream[i % _capacity];
        if (b) {
            if (c != data[j]) {
                throw std::runtime_error("an differente message");
            }
        } else {
            c = data[j], b = true;
            ++_size;
        }
    }

    string s;
    for (; _idx < _lastidx && _stream[_idx % _capacity].second; ++_idx) {
        s.push_back(_stream[_idx % _capacity].first);
        _stream[_idx % _capacity].second = false;
        --_size;
    }

    _output.write(s);
    if (_idx == _lastidx) _output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const { return _size; }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
