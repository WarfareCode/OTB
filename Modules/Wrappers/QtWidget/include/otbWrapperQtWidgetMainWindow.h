/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperQtWidgetMainWindow_h
#define otbWrapperQtWidgetMainWindow_h

#include <QMainWindow>
#include "OTBQtWidgetExport.h"

#include "otbWrapperApplication.h"

class QAction;
class QMenu;

namespace otb
{
namespace Wrapper
{

class QtWidgetView;

class OTBQtWidget_EXPORT QtMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  QtMainWindow(Application::Pointer app, QtWidgetView* gui, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

public slots:
  void UnhandledException(QString message);

private:
  QMenu*   helpMenu;
  QAction* helpAction;

  QtWidgetView* gui;
};

} // namespace Wrapper
} // namespace otb

#endif
