#ifndef FLAGGER_GENERIC_H
#define FLAGGER_GENERIC_H

#include <Phi/Functional/Either.h>
#include <Phi/Functional/Function.h>

#include <Std/Vector.h>

using namespace Phi;

template <class A>
Vector<A> mappend(Vector<A> a, Vector<A> b) {
    Vector<A> c { a };
    c.insert(std::end(c), std::begin(b), std::end(b));

    return c;
}

template <class A, class B>
Either<A, B> mappend(Either<A, B> a, Either<A,B> b) {
    if (isRight(a) && isRight(b)) {
        return mappend(fromRight(a), fromRight(b));
    } else {
        return a;
    }
}

template <class A, class B>
A foldL(Fn<A(A,B)> f, A&& ini, const Vector<B>& l) {

    A res = ini;
    for (auto elem : l) {
        res = f(res, elem);
    }

    return res;
}

#endif // EITHER_H
