#include "Item.h"

void CItem::SetModel(CModel::GEOMETRY_TYPE Newmodel)
{
	model.SetModel(Newmodel);
}
	
CModel::GEOMETRY_TYPE CItem::GetModel()
{
	return model.getModel();
}
	
void CItem::SetPrice(int NewPrice)
{
	price = NewPrice;
}

int CItem::GetPrice()
{
	return price;
}

void CItem::SetItemName(string NewName)
{
	ItemName = NewName;
}

string CItem::GetItemName()
{
	return ItemName;
}

void CItem::Set(CModel::GEOMETRY_TYPE NewModel, int NewPrice, string NewName)
{
	model.SetModel(NewModel);
	price = NewPrice;
	ItemName = NewName;
}