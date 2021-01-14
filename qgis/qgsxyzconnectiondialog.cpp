/***************************************************************************
    qgsxyzconnectiondialog.cpp
    ---------------------
    begin                : February 2017
    copyright            : (C) 2017 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsxyzconnectiondialog.h"
#include "qgsxyzconnection.h"

QgsXyzConnectionDialog::QgsXyzConnectionDialog( QWidget *parent )
  : QDialog( parent )
{
  setupUi( this );

}

void QgsXyzConnectionDialog::setConnection( const QgsXyzConnection &conn )
{
  mEditName->setText( conn.name );
  mEditUrl->setText( conn.url );
}

QgsXyzConnection QgsXyzConnectionDialog::connection() const
{
  QgsXyzConnection conn;
  conn.name = mEditName->text();
  conn.url = mEditUrl->text();
  return conn;
}
