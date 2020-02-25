#ifndef PARSER_H
#define PARSER_H

#include <deque>

#include <Phi/Functional/Either.h>

#include <Std/Utility.h>
#include <Smuggler/Error.h>
#include <Smuggler/BTypes/String.h>
#include <Smuggler/BTypes/Vector.h>

namespace Smuggler {

using Phi::Either;
using Token = String;

template <class T>
using Deque = std::deque<T>;

using T = std::tuple<String, size_t, size_t>;
using Elems = Pair<Vector<String>, size_t>;

Either<ParseError, Elems> tokenize(const String& time, const T& dlc);
Either<ParseError,Vector<Pair<String, Token>>> tokenize(const String& str, const Deque<Deque<T>> tokens);

}

#endif // PARSER_H
