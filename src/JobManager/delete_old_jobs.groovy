// Jenkins Groovy script to delete old jobs

// Usage: c:\Windows\SysWOW64\java.exe -jar jenkins-cli.jar -s http://localhost:8080 groovy delete_old_jobs.groovy
//  configure SSH key at http://jenkins/me/configure
//  get jenkins-cli.jar at http://jenkins/jnlpJars/jenkins-cli.jar

def old_timestamp = java.util.Calendar.getInstance()
old_timestamp.add(Calendar.DAY_OF_MONTH, -2)
// println old_timestamp.getTime()

def jobs = hudson.model.Hudson.instance.items

jobs.each { job ->
// http://javadoc.jenkins-ci.org/hudson/model/Job.html
    def delete_job = false;
    if (job.getLastBuild() == null) {
        // maybe dangerous
        // delete_job = true;
    } else {
        if (job.getLastBuild().getTimestamp().before(old_timestamp)) {
            delete_job = true;
        }
    }
    if (delete_job)
    {
        println "Job '${job.name}', last build " + (job.getLastBuild() != null ? job.getLastBuild().getTime().toString() : "never")
        job.doDoWipeOutWorkspace()
        job.delete()
    }
}
