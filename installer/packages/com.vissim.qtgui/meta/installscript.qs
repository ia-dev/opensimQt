function Component()
{
}

Component.prototype.createOperations = function()
{

    component.createOperations();
    var winpath = installer.environmentVariable("PATH") + ";" + installer.value("TargetDir")+"/bin";
    component.addElevatedOperation("EnvironmentVariable","PATH",winpath,true);
}