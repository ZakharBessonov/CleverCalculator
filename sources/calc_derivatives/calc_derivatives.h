#ifndef CALC_DERIVATIVES_HEADER
#define CALC_DERIVATIVES_HEADER

Node* TakeDerivativeAdd(Node* node, char varDifferentiation);

Node* TakeDerivativeSub(Node* node, char varDifferentiation);

Node* TakeDerivativeMul(Node* node, char varDifferentiation);

Node* TakeDerivativeDiv(Node* node, char varDifferentiation);

Node* TakeDerivativePow(Node* node, char varDifferentiation);

Node* TakeDerivativeLog(Node* node, char varDifferentiation);

Node* TakeDerivativeLn(Node* node, char varDifferentiation);

Node* TakeDerivativeSin(Node* node, char varDifferentiation);

Node* TakeDerivativeCos(Node* node, char varDifferentiation);

Node* TakeDerivativeTg(Node* node, char varDifferentiation);

Node* TakeDerivativeCtg(Node* node, char varDifferentiation);

Node* TakeDerivativeSh(Node* node, char varDifferentiation);

Node* TakeDerivativeCh(Node* node, char varDifferentiation);

Node* TakeDerivativeTh(Node* node, char varDifferentiation);

Node* TakeDerivativeCth(Node* node, char varDifferentiation);

Node* TakeDerivativeArcsin(Node* node, char varDifferentiation);

Node* TakeDerivativeArccos(Node* node, char varDifferentiation);

Node* TakeDerivativeArctg(Node* node, char varDifferentiation);

Node* TakeDerivativeArcctg(Node* node, char varDifferentiation);

#endif
