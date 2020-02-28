#include <Phi/Functional/Function.h>
#include <Phi/Functional/Functor.h>
#include <Phi/Iter/While.h>

#include <Smuggler/Common/Parser.h>

using namespace Phi;

namespace Smuggler {

String tokenNotFound(
    const String& str,
    const String& delim,
    const String& initialPos,
    const String& maxSize
) {
    String err =
        "Expected token '" + delim  +"' was not found in string '" + str + "', "
        "with offset '" + initialPos + "' and a max space of '" + maxSize + "'";

    return err;
}

Either<ParseError, Elems> tokenize(const String& time, const T& dlc) {
    const String delim = std::get<0>(dlc);
    const size_t maxSize = std::get<2>(dlc);
    size_t count = std::get<1>(dlc);

    ParseError pError = ParseError { 0, "" };
    Elems elems = { {}, 0 };
    bool error = false;

    size_t initialPos = 0;
    while (count > 0) {
        const String acSubstr = time.substr(initialPos, maxSize);
        const size_t delimPos = acSubstr.find(delim.c_str(), 0);

        if (delimPos == String::npos) {
            const String strInitialPos = std::to_string(initialPos);
            const String strMaxSize = std::to_string(maxSize);
            pError =
                ParseError {
                    initialPos,
                    tokenNotFound(time, delim, strInitialPos, strMaxSize)
                };
            error = true;
            break;
        } else {
            String elem = time.substr(initialPos, delimPos);
            initialPos += static_cast<size_t>(delimPos) + 1;

            elems.first.push_back( elem );
            elems.second = initialPos;

            count--;
        }
    }

    if (error) {
        return pError;
    } else {
        return elems;
    }
}

using DState = Pair<String, Deque<Deque<T>>>;
using State = Pair<DState, Vector<Pair<String, Token>>>;

Either<ParseError,Vector<Pair<String, Token>>> tokenize(const String& str, const Deque<Deque<T>> tokens) {
    State initial { { str, tokens }, {} };

    auto isFinal = Fn<bool(const State&)>([](const State& s) -> bool {
        auto rest { s.first.first };
        auto delims { s.first.second };

        if (rest.empty() && delims.empty()) {
            return true;
        } else {
            return false;
        }
    });

    auto compute = Fn<Either<ParseError, State>(State&&)> (
        [](State&& curState) -> Either<ParseError, State> {
            auto rest { curState.first.first };
            auto delims { curState.first.second };
            auto pResults { curState.second };

            if (delims.empty() || rest.empty()) {
                if (delims.empty() != true && rest.empty()) {
                    String tokensLeft { "[" };

                    while (!delims.empty()) {
                        tokensLeft += "[";
                        auto nextDelims { delims.front() };

                        while (!nextDelims.empty()) {
                            auto curDelims { nextDelims.front() };

                            tokensLeft.append( String(get<0>( curDelims ) + ",") );
                            nextDelims.pop_front();
                        }

                        tokensLeft += "]";
                        delims.pop_front();
                    }

                    tokensLeft += "]";

                    return ParseError {
                        -1,
                        u8"ParseError: Wrong date format, unexpected end of string. The "
                        u8"following tokens where missing: " + tokensLeft
                    };
                } else {
                    pResults.push_back( { u8"", rest } );
                    return State { { {}, {} }, pResults };
                }
            } else {
                auto curDelims { delims.front() };
                Either<ParseError, State> nextState { ParseError {-1, "No valid tokens found."} };

                for (const auto& delim : curDelims) {
                    auto eRes { tokenize(rest, delim) };

                    auto elemsToState { Fn<State(const Elems&)>(
                        [&delim, &rest, &delims, &pResults] (const Elems& elems) -> State {
                            size_t offset { elems.second };
                            Vector<String> parsed { elems.first };

                            String rRest {};
                            if (!rest.empty()) {
                                rRest = rest.substr(offset);
                            }

                            Deque<Deque<T>> rDelims {};
                            if (!delims.empty()) {
                                rDelims = { delims.begin() + 1, delims.end() };
                            }

                            Vector<Pair<String, Token>> updResults {};
                            updResults.insert(updResults.end(), pResults.begin(), pResults.end());

                            auto addTokens = Fn_([&delim](String pElem) -> Pair<String, Token> {
                                return Pair<String, Token> { pElem, std::get<0>(delim) };
                            });

                            Vector<Pair<String, Token>> parsedTokens { fmap(addTokens, parsed) };
                            updResults.insert(updResults.end(), parsedTokens.begin(), parsedTokens.end());

                            return State { { rRest, rDelims }, updResults };
                        }
                    )};

                    nextState = fmap(elemsToState, eRes);
                    if (isRight(nextState)) {
                        break;
                    }
                }

                return nextState;
            }
        }
    );

    auto eRes { While(move(initial), isFinal, compute) };

    if (isLeft(eRes)) {
        return fromLeft(eRes);
    } else {
        auto result = fromRight(eRes);
        return result.second;
    }
}

}
