#ifndef VSOSIM_H
#define VSOSIM_H

#include <QObject>
#include <OpenSim.h>
#include <PythonQtObjectPtr.h>
#include "vsModeling/vsNavigatorModel.h"
#include "vsPythonQt.h"
#include <qlogging.h>
#include <QDebug>
#include "vsTools/vsOpenSimTools.h"

//!
//! \brief The vsOsim class
//! vsOsim class is c++ extension module for Python Scripting
/*!
 * The vsOsim class provides the extension module and methods for the Python Scripting Environment embeded in the
 * vsOpenSimQt GUI scripting console.
 *
 * Following functionality is provided through extended methods of this class:
 *  - Loading, Creating, Initializing Models
 *  - Obtaining loaded Models from Navigator
 *  - Editing Models and its Components by using OpenSim python module
 *      - In order to use OpenSim functionality OpenSim python binding module should be installed in the system
 *      .
 *  - Updating Edited Models to navigator and use gui for visulation and simulation stuties
 *  .
 * This class is avialble as vsSim in the Python Scripting Console.
 */
class vsOsim : public QObject
{
    Q_OBJECT
public:
    vsOsim(QObject *parent, vsNavigatorModel *navigator) : QObject{parent}, nav{navigator} {}
    virtual ~vsOsim() {}

public slots:
    //!
    //! \brief getModelFromNavigator
    //! returns a copy of existing OpenSim::Model as Python Object opensim.Model
    //! \param modelName : name of the model already loaded in navigator and as given in the name property of the Model
    //! \return Python Object opensim.Model
    /*! Use this method to get a copy of the Model loaded in the navigator.
     * The function is declared as a method of class variable vsSim
     *
     * Usage in Python Scripting
     * \code{.py} m = vsSim.getModelFromNavigator('<modelname>') \endcode
     */
    PyObject* getModelFromNavigator(const QString modelName);

    //!
    //! \brief getCurrentModel
    //! returns a copy of the current model selected in the navigator
    //! \return Python Object opensim.Model
    /*!
     * Use this method to get a copy of the Current Active Model selected in the navigator
     * The function is declared as a method of class variable vsSim
     *
     * Usage in Python Scripting
     * \code{.py} m = vsSim.getCurrentModel() \endcode
     */
    PyObject* getCurrentModel();

    //!
    //! \brief setModelToNavigator
    //! Creates a copy of model (opensim.Model) created in Python Console and adds it to the Navigator
    //! \param py_model instance of the model of type opensim.Model or opensim.simulation.Model
    /*!
     * Use this method to add the model of type opensim.Model created in Python Script to
     * the navigator.
     * After adding the model to navigator all the gui functionality can be used
     * Note: If the model exists, it will be modified as per the current specifications and will be loaded to navigator.
     * Caution: Do not attempt to use this method with existing model file name if you do not intend to overwrite the original file.
     * The function is declared as a method of class variable vsSim
     *
     * Usage in Python Scripting
     * example:
     * \code{.py}
     * m = opensim.Model()                                                              # create a new opensim.Model
     * m.setName('mymodel')                                                             # set the name of the model
     * m.setInputFile('<path to the model file typically having extension .osim>')      # set the full path of the model file having extension .osim
     * vsSim.setModelToNavigator(m)                                                     # add the model to navigator
     * \endcode
     */
    void setModelToNavigator(PyObject* py_model);

    //!
    //! \brief updateModelToNavigator
    //! \param py_model instance of the model of type opensim.Model or opensim.simulation.Model
    /*!
     * Use this method to update changes to the model in navigator from the model of type opensim.Model, modified in Python Script
     * All the modifications made to the model will be used to update corresponding
     * properties of the model in navigator.
     *
     * Note: If the model exists, it will be modified as per the current specifications and will be loaded to navigator.
     * Caution: Do not attempt to use this method with existing model file name if you do not intend to overwrite the original file.
     * The function is declared as a method of class variable vsSim
     *
     * After updating the model to navigator all the gui functionality can be used. This is required step
     * if the modifications to model are indented to be updated and viewed in gui.
     *
     * The function is declared as a method of class variable vsSim
     *
     * Usage in Python Scripting
     * example:
     * \code{.py}
     * m = vsSim.getModelFromNavigator('<modelname>')                   # get the model by name from navigator
     * m.setName('mymodel')                                             # change the name of model
     * vsSim.updateModelToNavigator(m)                                  # update changes to the navigator model
     * \endcode
     */
    void updateModelToNavigator(PyObject* py_model);

    //!
    //! \brief loadModel
    //! Loads a model from a .osim file and returns the opensim.Model object
    //! \param filePath : the absolute path of a model file with extension .osim
    //! \return Python Object opensim.Model
    /*!
     * Use this method to load a model in navigator from a model file having extension .osim
     * An opensim.Model object is returned by the method. This object can be used in Python Script to modify the
     * properties of model.
     *
     * Use  vsOsim::updateModelToNavigator to update the changes done in Python Script to the model in navigator
     * After updating the model to navigator all the gui functionality can be used. This is required step
     * if the modifications to model are indented to be updated and viewed in gui.
     *
     * The function is declared as a method of class variable vsSim
     *
     * Usage in Python Scripting
     * example:
     * \code{.py}
     * m = vsSim.loadModel('<path to the model file typically having extension .osim>')      # set the full path of the model file having extension .osim
     * \endcode
     */
    PyObject* loadModel(const QString& filePath);

private:

    OpenSim::Model* findModel(QString const &modelName);
    OpenSim::Model* findModelByFileName(QString const &fileName);
    PythonQtObjectPtr getOpenSimModule() const;
private:
    vsNavigatorModel *nav;

    PythonQtObjectPtr createPythonModel(const QString &filePath);
};




#endif // VSOSIM_H
