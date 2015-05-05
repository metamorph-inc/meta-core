// cron usage:
// 0 5 * * * /usr/bin/timeout 4m java -jar jenkins-cli.jar -s http://localhost:9999/ groovy check_glew.groovy

def static Run(nodeName, runCommand)
{
    def output = new java.io.ByteArrayOutputStream();
    def listener = new hudson.util.StreamTaskListener(output);
    def node = hudson.model.Hudson.instance.getNode(nodeName);
    def launcher = node.createLauncher(listener);

    def command = new hudson.util.ArgumentListBuilder();
    command.addTokenized(runCommand);
    def ps = launcher.launch();
    ps = ps.cmds(command).stdout(listener);
    // ps = ps.pwd(build.getWorkspace()).envs(build.getEnvironment(listener));
    def proc = launcher.launch(ps);
    int retcode = proc.join();
    return [retcode, output.toString()]
}

for (nodeName in ["worker31", "worker32"])
{
(retval, output) = Run(nodeName, "cmd.exe /C cd %userprofile% && glew_x64\\glewinfo.exe")
if (retval != 0)
  println nodeName + ": glewinfo returned " + retval
(retval, output) = Run(nodeName, "cmd.exe /C type %userprofile%\\glewinfo.txt")
if (output.indexOf("GeForce") == -1)
  println nodeName + ": could not find GeForce"
  // println output
}
