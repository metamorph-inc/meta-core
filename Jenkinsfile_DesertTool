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

                bat($/.\src\.nuget\NuGet.exe restore externals\desert\desertVS2010.sln/$)
                bat($/"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" src\CyPhyMLCombined.sln  /p:Configuration=Release;Platform="Mixed Platforms" /t:DesertTool /m/$)

                archiveArtifacts artifacts: 'externals/desert/bin/desert.dll,externals/desert/DesertDll/Release/desert.pdb,externals/desert/bin/DesertTool.exe,externals/desert/bin/DesertTool.pdb', onlyIfSuccessful: true
            }

        }
    }
}

// set GIT_ALTERNATE_OBJECT_DIRECTORIES=c:\meta-core\.git\objects
