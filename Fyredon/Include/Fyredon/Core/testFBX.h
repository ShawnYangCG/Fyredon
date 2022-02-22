#pragma once
#include <iostream>
#include <stdio.h>
#include <fbxsdk.h>
class testFBX
{
public:
	int numTabs = 0;/* Tab character ("\t") counter */
public:
	testFBX();
	void PrintTabs();
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);
	void PrintAttribute(FbxNodeAttribute* pAttribute);
	void PrintNode(FbxNode* pNode);

};

