#===========================================================================
#                           opensimQt Builder
#===========================================================================
#
# This script provides a way to automatically pull and build opensimQt
# currently support Ubuntu , tested on Ubuntu 18.04, 20.04
#
#  How to use:
#                  sudo python3 buildOpenSimQt.py
#
#__author__ = "Alexander Weck, , , , "
#__copyright__ = ""
#__credits__ = [""]
#__license__ = ""
#__version__ = "1.0.0"
#__maintainer__ = "Alexander Weck"
#__email__ = "ajani@uni-koblenz.de"
#__status__ = "Development"

# TODOS:
#  remove temps opensim-dev.zip opensimQt-master
#  mainpulate user path
#   export PATH=$PATH:/home/ibr/opensim-dev/opensim-core-4.1-install
#   export PATH=$PATH:/home/ibr/opensim-dev/opensim-core-4.1-install/bin
#   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ibr/opensim-dev/opensim-core-4.1-install/lib
# uploade updated opensimqt cmake
# upload the new script to be tested on windows 

#
#  build small tools from source
#  test on 18.04, and 20.04

# All important import
from shutil import which
from enum import Enum
import os , sys , subprocess, shutil, time, fileinput , apt, platform , requests, zipfile
import ctypes

OpSys = Enum('OpSys', 'Windows Linux Darwin Unknown')
this_system = OpSys[platform.system()]
pyVer = platform.python_version()[0 : 3]

print("------------------------------------------")
print("------       User Parameters     ---------")
print("------------------------------------------")
numCores = 4 # number of cpu threads
# what to build:
doBuildReq             = 1
doBuildVTK             = 1
doBuildOpenSim         = 1
doBuildPythonQt        = 1
doBuildOpenSimQt       = 1

# where to build:
cUser     = os.getenv("SUDO_USER")
buildPath = os.path.join("/home",cUser,"opensim-dev")  

#---------------------------------------------------------------------
#                     Required functions
#---------------------------------------------------------------------
def vsCreateFolder(folderPath):       
    print(folderPath)                                                                            
    if not os.path.isdir(folderPath):
       os.mkdir(folderPath) 
       shutil.chown(folderPath, user=cUser, group=cUser)
    else:
       print("folder exist: ",folderPath)

def vsPip(packageName):       
   #TODO: add windows support
   subprocess.call(["sudo","pip3","install","packageName"])

def vsConfig(cmakePath,cmakeCfg):       
   #TODO: add windows support
   os.chdir(cmakePath)   
   subprocess.call(cmakeCfg)

def vsBuild(buildPath,do_make_install=None,useQmake=0,buildDoc=0):
   #TODO: add windows support
   if useQmake:
      projectPath = buildPath
      buildPath = os.path.abspath(os.path.join(buildPath, os.pardir))+"-build"
      print(buildPath)
      subprocess.call(["qmake",projectPath],cwd=buildPath)
   os.chdir(buildPath)
   subprocess.call(["make","-j",str(numCores)])
   if buildDoc:
      subprocess.call(["make","doxygen","-j",str(numCores)])   
   if do_make_install == True:
      subprocess.call(['make','install'])
      
   vsChownR(buildPath,  cUser)

    
def vsAddPath(pathLst,pathType):
   #TODO: add windows support
   bashFilePath= os.path.join("/home",cUser,".bashrc")
   #path type 0: path, 1:lib
   pathName="PATH" if pathType==0 else "LD_LIBRARY_PATH"
   pathName="'export "+pathName+"=$"+pathName
   new_path=""
   for x in pathLst:
       new_path=new_path+":"+x
       pathName = pathName +new_path
   cmd = "echo "+pathName+"' >> "+ bashFilePath    
   print(cmd)    
   #open .bashrc

   #subprocess.call(  [cmd])
   #s.environ["PATH"] += os.pathsep + new_path
   #source .bashrc
   #subprocess.call(["source "+bashFilePath],shell=True,executable='/bin/bash')   
   subprocess.call(['/bin/bash', '-i', '-c', "source "+bashFilePath])
   shutil.chown(bashFilePath, cUser)

def vsChownR(path, owner):
       for dirpath, dirnames, filenames in os.walk(path):
           shutil.chown(dirpath, owner)
           for filename in filenames:
               shutil.chown(os.path.join(dirpath, filename), owner)

def vsDownload(url, downloadPath, chunk_size, targetPath, removeOld=0):
       #chunk_size=128
       if removeOld:
          print("removing old folder ",targetPath)
          shutil.rmtree(targetPath)
       if not os.path.isdir(targetPath) :
           print("downloading ",url) 
           r = requests.get(url, stream=True)
           zipFilePath =downloadPath+".zip"
           with open(zipFilePath, 'wb') as fd:
                for chunk in r.iter_content(chunk_size):
                   fd.write(chunk)
           zip_ref = zipfile.ZipFile(zipFilePath, 'r')
           extracted = zip_ref.namelist()
           zip_ref.extractall(downloadPath)
           zip_ref.close()
           extracted_folder = os.path.join(downloadPath, extracted[0])
           print(extracted_folder)
           #folderName = os.listdir(downloadPath)[0]
           try:
              shutil.copytree(os.path.join(downloadPath,extracted_folder),targetPath)
              shutil.rmtree(extracted_folder)
           except:
              print("folder exist, no need to copy ...")
              vsChownR(targetPath,  cUser)
       else:
            print("target ",targetPath, " exist, no need to download ...")

print("# ===============================================")
print("#           Start Building  opensimQt            ")
print("#================================================")
vsCreateFolder(buildPath)

#---------------------------------------------------------------------
#                     Build required tools
#---------------------------------------------------------------------
if doBuildReq:    
   btS=time.time()
   print("Install all defined required libs ..................")
   #TODO: add offline links, add specific versions 
   #  Ubuntu20: doxygen 1.8.17, cmake 3.16.3
   #List of all packages that will be installed by apt.
   reqLst = "dialog build-essential doxygen swig3.0 curl python3-pip python3-dev git cmake liblapack-dev libgl1-mesa-dev xorg-dev mesa-common-dev libgl1-mesa-dev libgles2-mesa-dev freeglut3-dev".split()

   reqQt5Lst="qt5-default qttools5-dev-tools qtcreator qttools5-dev libqt5webengine5 qtbase5-private-dev libqt5svg5-dev libqt5xmlpatterns5-dev libqt5multimedia5 qtmultimedia5-dev qml qtwebengine5-dev libqt5designer5 qtdeclarative5-dev".split()
   allReqLst =reqLst; reqLst.extend(reqQt5Lst)
   for pkg_name in allReqLst:
       """Install package from apt with name 'pkg_name'. Returns True if is/was installed"""
       cache = apt.cache.Cache(); cache.update();cache.open()
       pkg = cache[pkg_name]
       if pkg.is_installed:
          print("{pkg_name} already installed".format(pkg_name=pkg_name))
       else:
          pkg.mark_install()
       try:
          cache.commit()
       except  vsErr:
          print ( "Sorry, package installation failed: ", vsErr  )
   #TODO: replace download by git clone if possible       e.g.
      #git.Git(os.path.join(vtkPath,"vsSrc")).clone(vtkURL)
      #Repo.clone_from(vtkURL,srcPath )
   #subprocess.call(["sudo","pip3","install","gitpython"])
   #vsPip("gitpython")

   btE=time.time();   bt =  btE - btS;    print("installing requred tools took ",bt/60.0," minutes")
else:
    print("Install required tools is disabled. ")

from git import Repo

#---------------------------------------------------------------------
#          Setup Paths, WebLinks, and Cmake configs
#---------------------------------------------------------------------
#Constants, weblinks and cmake configueration

# Checks if executing user is admin/root
is_admin=False
if this_system == OpSys.Windows:
    is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
elif this_system == OpSys.Linux:
      is_admin = os.getuid() == 0
elif this_system == OpSys.Darwin:
        is_admin = False #NotYetImplemented       
if not is_admin:
    print("You need elevated rights to execute this script e.g. use sudo in Ubuntu.")
    sys.exit()

tbtStart=time.time()

#--------------------------------------------------------------------------
#                                        OpenSimDev
#--------------------------------------------------------------------------
vsCreateFolder(buildPath)

#--------------------------------------------------------------------------
#                                        VTK
#--------------------------------------------------------------------------
vtkPath       = os.path.join(buildPath,"VTK-9.0.0.rc3")
vtkBuildPath  = os.path.join(vtkPath,"build")
if doBuildVTK:
    print("building VTK .............................")
    btS=time.time()
    vtkGit        = "https://github.com/Kitware/VTK.git" ; vtkTag="v9.0.0.rc3"
    vtkURL        = "https://github.com/Kitware/VTK/archive/v9.0.0.rc3.zip"

    vtkSrcPath    = os.path.join(vtkPath,"src")
    vtkCmakeCfg   = ["cmake",vtkSrcPath,"-DVTK_PYTHON_VERSION='3'", "-DVTK_MODULE_ENABLE_VTK_GUISupportQt='YES'" , "-DVTK_MODULE_ENABLE_VTK_RenderingQt='YES'"
                  ,"-DVTK_GROUP_ENABLE_Qt='YES'","-DVTK_MODULE_ENABLE_VTK_ViewsQt='YES'", "-DCMAKE_BUILD_TYPE='Release'"]
    vsCreateFolder(vtkPath)
    vsDownload(vtkURL, vtkPath, 128,vtkSrcPath)
    vsCreateFolder(vtkBuildPath);    vsConfig(vtkBuildPath,vtkCmakeCfg) ;    vsBuild(vtkBuildPath)
    #vsAddPath([os.path.join(vtkBuildPath,"bin")],0)
    btE=time.time() ;    bt =  btE - btS ;     print("building VTK took ",bt/60.0," minutes")
else:
    print("building VTK is disabled .............................")

#--------------------------------------------------------------------------
#                                        OPENSIM-CORE
#--------------------------------------------------------------------------
opensimInstallPath     = os.path.join(buildPath,"opensim-core-4.1-install")
if doBuildOpenSim:
    print("building OpenSim .............................")
    btS=time.time()
    opensimGit        = "" ; opensimTag=""
    #opensimURL             = "https://github.com/opensim-org/opensim-core/archive/4.1.zip"
    opensimURL             = "https://github.com/VisSimKoblenz/opensim-core/archive/master.zip"
    opensimSrcPath         = os.path.join(buildPath,"opensim-core-4.1")
    opensimBuildPath       = os.path.join(buildPath,"opensim-core-4.1-build")
    opensimDepsSrcPath     = os.path.join(opensimSrcPath,"dependencies")
    opensimDepsBuildPath   = os.path.join(buildPath,"opensim-deps-4.1-build")
    opensimDepsInstallPath = os.path.join(buildPath,"opensim-deps-4.1-install")    
    simbodyBuildSrc        = os.path.join(opensimDepsSrcPath,"simbody")
    simbodyBuildPath       = os.path.join(opensimDepsBuildPath,"simbody","build")
    opensimDepsCfg   = ["cmake",opensimDepsSrcPath,"-DSUPERBUILD_BTK='True'", "-DSUPERBUILD_docopt='True'","-DSUPERBUILD_simbody='True'","-DCMAKE_INSTALL_PREFIX="+opensimDepsInstallPath, "-DCMAKE_BUILD_TYPE='Release'"]
    simbodyCfg   = ["cmake",simbodyBuildPath,"-DBUILD_EXAMPLES=ON"]
    opensimCoreCfg   = ["cmake",opensimSrcPath,"-DSUPERBUILD_BTK='True'", "-DBUILD_PYTHON_WRAPPING='True'", "-DOPENSIM_PYTHON_VERSION='3'","-DOPENSIM_DEPENDENCIES_DIR="+opensimDepsInstallPath,"-DCMAKE_INSTALL_PREFIX="+opensimInstallPath, "-DCMAKE_BUILD_TYPE='Release'"]
    vsDownload(opensimURL, buildPath, 128, opensimSrcPath)
    #vsCreateFolder(opensimDepsBuildPath) ;  
    vsConfig(opensimDepsBuildPath,opensimDepsCfg) ;
    vsBuild(opensimDepsBuildPath)
    # build simbody examples and documentation 
    vsConfig(simbodyBuildPath,simbodyCfg) ;    vsBuild(simbodyBuildPath,1,0,1) ;    vsChownR(opensimDepsInstallPath, cUser)
    vsCreateFolder(opensimBuildPath) ;    vsConfig(opensimBuildPath,opensimCoreCfg) ;    vsBuild(opensimBuildPath,1,0,1)        
    #  opensimPaths    = [opensimInstallPath, os.path.join(opensimInstallPath,"bin")]
    #  opensimLibPaths = [os.path.join(opensimInstallPath,"lib")]
    #vsAddPath(opensimPaths,0)  ; vsAddPath(opensimLibPaths,1)
    #install opensim python e.g. /usr/local/lib/python3.8/dist-packages/opensim-4.1-py3.8.egg/
    os.chdir(os.path.join(opensimInstallPath,"lib","python"+pyVer,"site-packages"))
    subprocess.call(['python3','setup.py','install'])
    vsChownR(opensimInstallPath,cUser)
    btE=time.time() ;    bt =  btE - btS ;     print("building opensim-core took ",bt/60.0," minutes")
else:
        print("building OpenSim is disabled .................")

#--------------------------------------------------------------------------
#                                        PYTHONQT
#--------------------------------------------------------------------------
pythonQtPath       = os.path.join(buildPath,"pyhtonQT-3.2")
pythonQtBuildPath  = os.path.join(buildPath,"pyhtonQT-3.2-build")
if doBuildPythonQt:
        print("building PythonQt .............................")
        btS=time.time()
        pythonQtURL        = "https://github.com/VisSimKoblenz/pythonqt/archive/master.zip"
        vsDownload(pythonQtURL, buildPath, 128, pythonQtPath)
        python_prf_path = os.path.join(pythonQtPath,"build","python.prf")
        pythonQtProjectPath=os.path.join(pythonQtPath,"PythonQt.pro")
        
        # modify python verion       
        pyText= "win32:PYTHON_VERSION" if this_system == OpSys.Windows else "unix:PYTHON_VERSION"
        newLine = "  "+pyText+"="+pyVer+"\n"
        fin = open(python_prf_path,"r");     fout = open("tmp.txt","w")
        for line in fin.readlines():
            line=newLine if pyText in line else line                
            fout.write(line)
        fin.close() ;        fout.close()
        os.remove(python_prf_path); shutil.move("tmp.txt", python_prf_path);   shutil.chown(python_prf_path,cUser)
        
        vsCreateFolder(pythonQtBuildPath);
        vsBuild(pythonQtProjectPath,None,1) ;  vsChownR(pythonQtBuildPath,  cUser)
        
        btE=time.time() ;        bt =  btE - btS;   print("building pythonQt took ",bt/60.0," minutes")
else:
        print("building PythonQt is disabled .............................")

 
#--------------------------------------------------------------------------
#                                        OPENSIMQT
#--------------------------------------------------------------------------
if doBuildOpenSimQt:
        print("building OpenSimQt .............................")
        btS=time.time()
        opensimQtURL        = "https://github.com/VisSimKoblenz/opensimQt/archive/master.zip"
        opensimModelsURL    =" https://github.com/VisSimKoblenz/opensim-models/archive/master.zip"
        opensimQtPath       = os.path.join(buildPath,"opensimQt")
        opensimQtSrcPath    = os.path.join(opensimQtPath,"Gui")
        opensimQtBuildPath  = os.path.join(opensimQtPath,"build")
        opensimDir          = os.path.join(opensimInstallPath,"lib","cmake","OpenSim")
        opensimWSPath       = os.path.join(opensimQtPath,"vsWorkSpace")

        print(opensimDir)
        PYTHONQT_SRC_PATH   = pythonQtPath
        PYTHONQT_PATH       = pythonQtBuildPath
        opensimQtCmakeCfg   = ["cmake",opensimQtSrcPath,"-DOpenSim_DIR="+opensimDir,"-DVTK_DIR="+vtkBuildPath,"-DPyVer="+pyVer,"-DPYTHONQT_SRC_PATH="+PYTHONQT_SRC_PATH ,"-DPYTHONQT_PATH="+PYTHONQT_PATH,"-DCMAKE_BUILD_TYPE='Release'", "-DDownload_Geometry='True'"]
        vsDownload(opensimQtURL, buildPath, 128, opensimQtPath)
        vsDownload(opensimModelsURL, buildPath, 128, opensimWSPath)        
        vsCreateFolder(opensimQtBuildPath);  
        vsConfig(opensimQtBuildPath,opensimQtCmakeCfg) ;   
        vsBuild(opensimQtBuildPath,0,0,1)
        btE=time.time();  bt =  btE - btS;    print("building opensimQt took ",bt/60.0," minutes")
else:
        print("building OpenSimQt is disabled .............................")

tbtEnd=time.time()
tbt =  tbtEnd - tbtStart
print("building everything took ",tbt/60.0," minutes")

print("===========================================================")
print("    Buiding opensimQt is completed !")
print("===========================================================")





