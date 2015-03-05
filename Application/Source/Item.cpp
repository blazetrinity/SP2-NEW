/******************************************************************************/
/*!
\file	Item.cpp
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Sets the Items
*/
/******************************************************************************/

#include "Item.h"

/***********************************************************/
/*!
\brief
	Sets the Item Model

\param Newmodel
	Sets the new Model for the Item
*/
/***********************************************************/
void CItem::SetModel(CModel::GEOMETRY_TYPE Newmodel)
{
	model.SetModel(Newmodel);
}
	
/***********************************************************/
/*!
\brief
	Gets the Item Model

\return
	Returns the Item Model
*/
/***********************************************************/
CModel::GEOMETRY_TYPE CItem::GetModel()
{
	return model.getModel();
}
	
/***********************************************************/
/*!
\brief
	Sets the new Price for the Items

\param NewPrice
	Gives the Item a new Price
*/
/***********************************************************/
void CItem::SetPrice(int NewPrice)
{
	price = NewPrice;
}

/***********************************************************/
/*!
\brief
	Gets the Item Price

\return
	Returns the item price
*/
/***********************************************************/
int CItem::GetPrice()
{
	return price;
}

/***********************************************************/
/*!
\brief
	Sets the Item Name 

\param NewName
	Gives the item a new name
*/
/***********************************************************/
void CItem::SetItemName(string NewName)
{
	ItemName = NewName;
}

/***********************************************************/
/*!
\brief
	Gets the Item Name

\return 
	Gets the new Name for the Item
*/
/***********************************************************/
string CItem::GetItemName()
{
	return ItemName;
}

/***********************************************************/
/*!
\brief
	Sets the new model, new price and new name for the Item

\param NewModel
	Sets the new model for the Item

\param NewPrice
	Sets the new price for the Item

\param NewName
	Sets the new name for the Item
*/
/***********************************************************/
void CItem::Set(CModel::GEOMETRY_TYPE NewModel, int NewPrice, string NewName)
{
	model.SetModel(NewModel);
	price = NewPrice;
	ItemName = NewName;
}