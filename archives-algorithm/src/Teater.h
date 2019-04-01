#pragma once
#include "stdafx.h"
#include "./linear/LinkedList.h"
#include "./linear/Stack.h"
#include "./linear/Queue.h"

#include "./nonlinear/TreeObject.h"
#include "./nonlinear/VirtualTree.h"
#include "./nonlinear/HuffmanTree.h"
#include "./nonlinear/Graph.h"
#include "./nonlinear/GraphTemplate.h"

#include "./utility/Transition.h"
#include "./utility/BigInteger.h"
#include "./utility/SoftwareTest.h"

#include "ExtendSpace.h"
#include "MathLibrary.h"

// 用于梅森随机数
#include <random>

int runRadixTest();
int runMathTest();
int runDataStructureTest();
int runRandomTest();
