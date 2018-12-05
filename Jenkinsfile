def CAD_REV = 'UNKNOWN'

pipeline {
    agent none
    options {
        buildDiscarder(logRotator(numToKeepStr: '1000', artifactNumToKeepStr: '3'))
    }
    stages {
        stage('Build') {
            agent {
                node {
                    label 'meta-build'
                    customWorkspace 'c:\\jenkins_meta'
                }
            }
            steps {
                checkout scm
                bat($/cmd /c set/$)
                bat($/del deploy\META_*.exe/$)

                dir('deploy') {
                    script {
                        CAD_REV = bat (script: '@..\\bin\\Python27\\Scripts\\python -c "import vc_info; print vc_info.last_cad_rev()"', returnStdout: true).trim()
                    }
                }
                bat($/"c:\Program Files\Git\Usr\bin\find.exe" src/CADAssembler -iname META.\*.nupkg ! -iname META.\*${CAD_REV}.nupkg -print -delete/$)
                bat($/cmd /c register_interpreters.cmd || git clean -xdf/$)
                bat($/"c:\Program Files\Git\Usr\bin\find.exe" -iname \*UnmanagedRegistration.cache -print -delete/$)
                bat($/Setlocal EnableDelayedExpansion
c:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild make.msbuild /t:DownloadNuGetPackages && exit /b 0


c:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild make_CAD.msbuild /t:All /fl /m /nodeReuse:false || exit /b !ERRORLEVEL!

pushd src\CADAssembler
cmd /c ..\..\bin\Python27\Scripts\Python.exe CADCreoParametricCreateAssembly\build_nuget_package.py pack_nuget || exit /b !ERRORLEVEL!
cmd /c ..\..\bin\Python27\Scripts\Python.exe ExtractACM-XMLfromCreoModels\build_nuget_package.py pack_nuget || exit /b !ERRORLEVEL!
cmd /c ..\..\bin\Python27\Scripts\Python.exe CADCreoParametricMetaLink\build_nuget_package.py pack_nuget || exit /b !ERRORLEVEL!
popd

run_cadunittests.cmd/$)

                bat('jenkins_build.cmd')

                archiveArtifacts artifacts: 'deploy/META*exe,src/CADAssembler/*/META.*.nupkg', onlyIfSuccessful: true
                junit keepLongStdio: true, testResults: 'test/junit_results.xml'
            }

        }
    }
}

// set GIT_ALTERNATE_OBJECT_DIRECTORIES=c:\meta-core\.git\objects
