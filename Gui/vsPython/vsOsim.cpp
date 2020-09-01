#define PY_SSIZE_T_CLEAN
#include "vsOsim.h"


PyObject* vsOsim::getModelFromNavigator(const QString modelName)
{
    try {

        OpenSim::Model *model = findModel(modelName);
        if(!model){
            QString const msg = "print('Model: " + modelName + " does not exists!')";
            getOpenSimModule().evalScript(msg);
            return NULL;
        }

        return createPythonModel(QString::fromStdString(model->getInputFileName()));

//        //get the opensim module
//        vsPythonQt* pvsPythonQt = static_cast<vsPythonQt*>(this->parent());
//        PythonQtObjectPtr pOpenSimModule = pvsPythonQt->getOpenSimModule();

//        std::string const fileName = model->getInputFileName();
//        PythonQtObjectPtr pModelClass = PythonQt::self()->lookupCallable(pOpenSimModule,"opensim.Model");
//        QVariantList l;
//        QVariant a{QString::fromStdString(fileName)};
//        l.push_back(a);
//        PythonQtObjectPtr res = PythonQt::self()->callAndReturnPyObject(pModelClass,l);

//        return  res;

    } catch (std::exception &e) {
        qDebug() << e.what();
        return NULL;
    }


    //return model
}

PyObject* vsOsim::getCurrentModel()
{
    auto model = nav->getActiveModel();
    if(!model){
       getOpenSimModule().evalScript("print('No active Models found!')");
       return NULL;
    }
    return  getModelFromNavigator(QString(model->getName().c_str()));

}

void vsOsim::updateModelToNavigator(PyObject* py_model)
{

     setModelToNavigator(py_model);

}

PyObject* vsOsim::loadModel(const QString &filePath)
{
    try {
        //check if the model is already loaded
        auto model = findModelByFileName(filePath);
        if(model){
            return getModelFromNavigator(QString::fromStdString(model->getName()));
        }else{
            //load or reload the model
            OpenSim::Model  *newModel = new OpenSim::Model(filePath.toStdString());

            nav->loadOpenSimModel(newModel);


            //update the openModelsFile
            vsOpenSimTools::tools->addToOpenModels(newModel);
            return createPythonModel(filePath);
        }

    } catch (...) {
        vsOpenSimTools::tools->log("No OpenSim model created from Python","",vsOpenSimTools::Error,true);
        std::string const msg = "Could not load the model from file path :" + filePath.toStdString();
        PyErr_SetString(PyExc_Exception,msg.c_str());
        return NULL;
    }
}


void vsOsim::setModelToNavigator(PyObject* py_model)
{
    try {

        PythonQtObjectPtr pModel(py_model);
        //QVariant modelName = pModel.call("getName");
        QVariant vfileName = pModel.call("getInputFileName");

        //check if the model file is not present
        if(vfileName.toString() == "Unassigned"){
            //the model has no file.
            PyErr_SetString(PyExc_Exception,"No Input file assigned to the model. Please use setInputFileName(<full file path>)");
            return;
        }

        //check if the model is already loaded
        auto model = findModelByFileName(vfileName.toString());
        //write to model file .osim

        QVariantList l;
        l.push_back(vfileName);
        pModel.call("printToXML",l);                                //Update all the changes to model done in python console

        if(model){

            nav->closeModel(model);                                 //close exisiting model
        }

        //load or reload the model
        OpenSim::Model  *newModel = new OpenSim::Model(vfileName.toString().toStdString());

        nav->loadOpenSimModel(newModel);


        //update the openModelsFile
        vsOpenSimTools::tools->addToOpenModels(newModel);

    } catch (std::exception& e) {
        vsOpenSimTools::tools->log("No OpenSim model created from Python","",vsOpenSimTools::Error,true);
        PyErr_SetString(PyExc_Exception,e.what());
    }
}

OpenSim::Model *vsOsim::findModel(const QString &modelName)
{
    try {
        //get models
        auto models = nav->getOpenModels();
        //OpenSim::Model*m = models.first();

        //search the list of models by string
        QList<OpenSim::Model*>::iterator itr = std::find_if(models.begin(),models.end(),

                [&modelName](OpenSim::Model*& model)
                {
                    return model->getName() == modelName.toStdString();
                });

        if(itr == models.end()){
            return NULL;
        }
         return *itr;
    } catch (...) {

        throw;
    }

}

OpenSim::Model *vsOsim::findModelByFileName(const QString &fileName)
{
    try {
        //get models
        auto models = nav->getOpenModels();
        //OpenSim::Model*m = models.first();

        //search the list of models by string
        QList<OpenSim::Model*>::iterator itr = std::find_if(models.begin(),models.end(),

                [&fileName](OpenSim::Model*& model)
                {
                    return model->getInputFileName() == fileName.toStdString();
                });

        if(itr == models.end()){
            return NULL;
        }
         return *itr;
    } catch (...) {

        throw;
    }

}

PythonQtObjectPtr vsOsim::getOpenSimModule() const
{
    //get the opensim module
    vsPythonQt* pvsPythonQt = static_cast<vsPythonQt*>(this->parent());
    PythonQtObjectPtr pOpenSimModule = pvsPythonQt->getOpenSimModule();
    return pOpenSimModule;
}

PythonQtObjectPtr vsOsim::createPythonModel(const QString& filePath){

    vsPythonQt* pvsPythonQt = static_cast<vsPythonQt*>(this->parent());
    PythonQtObjectPtr pOpenSimModule = pvsPythonQt->getOpenSimModule();


    PythonQtObjectPtr pModelClass = PythonQt::self()->lookupCallable(pOpenSimModule,"opensim.Model");
    QVariantList l;
    QVariant a{filePath};
    l.push_back(a);
    PythonQtObjectPtr res = PythonQt::self()->callAndReturnPyObject(pModelClass,l);

    return  res;
}
