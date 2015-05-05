// usage:  java -jar jenkins-cli.jar -s http://10.2.204.106:9999/ groovy check_swri_env.groovy

for (aSlave in hudson.model.Hudson.instance.slaves) {
  env = aSlave.getComputer().getEnvironment();
  if (env.containsKey("SystemRoot") && env.containsKey("SWRI_BALLISTIC_EXTENSIONS") == false) {
   println('Worker has no SWRI_BALLISTIC_EXTENSIONS: ' + aSlave.name);
  }
}
