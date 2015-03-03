#ifndef ITEM_H
#define ITEM_H

#include "Model.h"
#include <string>

using std::string;

class CItem
{
public:
	void SetModel(CModel::GEOMETRY_TYPE Newmodel);
	CModel::GEOMETRY_TYPE GetModel();
	void SetPrice(int NewPrice);
	int GetPrice();
	void SetItemName(string NewName);
	string GetItemName();
	void Set(CModel::GEOMETRY_TYPE NewModel, int NewPrice, string NewName);

private:
	CModel model;
	int price;
	string ItemName;
};

#endif