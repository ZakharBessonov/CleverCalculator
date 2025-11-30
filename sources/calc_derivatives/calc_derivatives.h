#ifndef CALC_DERIVATIVES_HEADER
#define CALC_DERIVATIVES_HEADER

Node* TakeDerivativeAdd(Node* node);

Node* TakeDerivativeSub(Node* node);

Node* TakeDerivativeMul(Node* node);

Node* TakeDerivativeDiv(Node* node);

Node* TakeDerivativePow(Node* node);

Node* TakeDerivativeLog(Node* node);

Node* TakeDerivativeLn(Node* node);

Node* TakeDerivativeSin(Node* node);

Node* TakeDerivativeCos(Node* node);

Node* TakeDerivativeTg(Node* node);

Node* TakeDerivativeCtg(Node* node);

Node* TakeDerivativeSh(Node* node);

Node* TakeDerivativeCh(Node* node);

Node* TakeDerivativeTh(Node* node);

Node* TakeDerivativeCth(Node* node);

Node* TakeDerivativeArcsin(Node* node);

Node* TakeDerivativeArccos(Node* node);

Node* TakeDerivativeArctg(Node* node);

Node* TakeDerivativeArcctg(Node* node);

#endif
