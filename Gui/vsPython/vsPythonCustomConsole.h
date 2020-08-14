#ifndef VSPYTHONCUSTOMCONSOLE_H
#define VSPYTHONCUSTOMCONSOLE_H

//----------------------------------------------------------------------------------
/*!
// \file    vsPythonCustomConsole.h
// \author  Ritesh Sangani
// \author  Last changed by $Author: Ritesh $
// \date    2020-08-13
*/
//----------------------------------------------------------------------------------

#include "PythonQt.h"
#include <QVariant>
#include <QTextEdit>
#include <QMimeData>

class QCompleter;

//-------------------------------------------------------------------------------
//! Python Scripting Console
class vsPythonCustomConsole : public QTextEdit
{
  Q_OBJECT

public:
  vsPythonCustomConsole(QWidget* parent, const PythonQtObjectPtr& context);

  ~vsPythonCustomConsole();

signals:
  void historyUpdated(const QString history);

public Q_SLOTS:
  //! execute one line of script, typically one statement
  //! \param set storeOnly to true if multiline code execution is desired
  //! multiline code is executed when storeOnly is set to false
  void executeLine(bool storeOnly);

  //! derived key press event
  void keyPressEvent (QKeyEvent * e);

  //! output from console
  void consoleMessage(const QString & message);

  //! get history
  QStringList history() { return _history; }

  //! set history
  void setHistory(const QStringList& h) { _history = h; _historyPosition = 0; }

  //! clear the console
  void clear();

  //! overridden to control which characters a user may delete
  virtual void cut();

  //! output redirection
  void stdOut(const QString& s);
  //! output redirection
  void stdErr(const QString& s);

  void insertCompletion(const QString&);

  //! Appends a newline and command prompt at the end of the document.
  void appendCommandPrompt(bool storeOnly = false);

public:
  //! returns true if python cerr had an error
  bool hadError() { return _hadError; }

  //! returns true if python cerr had an error
  void clearError() {
    _hadError = false;
  }

  //! evaluates a python script file
  void evalFile(const QString& fileName);

protected:
  //! handle the pressing of tab
  void handleTabCompletion();

  //! Returns the position of the command prompt
  int commandPromptPosition();

  //! Returns if deletion is allowed at the current cursor
  //! (with and without selected text)
  bool verifySelectionBeforeDeletion();

  //! Sets the current font
  void setCurrentFont(const QColor& color = QColor(0,0,0), bool bold = false);

  //! change the history according to _historyPos
  void changeHistory();

  //! flush output that was not yet printed
  void flushStdOut();

  //! canInsertFromMimeData of QTextEdit overridden to control paste operation in the console
  //! Only html and text is allowed to be pasted
  bool canInsertFromMimeData(const QMimeData* source) const override;

  //! Override of the QTextEdit method, to manipulate the pasted source
  //! Executes the block of python script pasted in console
  void insertFromMimeData(const QMimeData *source) override;


private:
  void executeCode(const QString& code);

  PythonQtObjectPtr _context;

  QStringList _history;
  int         _historyPosition;

  QString _clickedAnchor;
  QString _storageKey;
  QString _commandPrompt;

  QString _currentMultiLineCode;

  QString _stdOut;
  QString _stdErr;

  QTextCharFormat _defaultTextCharacterFormat;
  QCompleter* _completer;

  bool _hadError;
};



#endif // VSPYTHONCUSTOMCONSOLE_H
