/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPropertyItem.h"

vsPropertyItem::vsPropertyItem():QStandardItem(),m_name("name"),m_value("value"),m_type(PropertyType::Text),m_isEditable(false)
{

}
