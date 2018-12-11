pipeline {
  agent any
  stages {
    stage('Get testing environment') {
      steps {
        echo 'RTAlib Jenkins testing pipeline'
        sh 'singularity exec ../images/rtalib-env.sigm python --version'
      }
    }
    stage('Creating config file') {
      steps {
        sh '''cat >rtalibconfig_testing.conf <<EOL
[General]
modelname=rtalib_dl_test_table
mjdref=
debug=no
batchsize=1
numberofthreads=1

[Dtr]
active=no
debug=no
inputchannel=

[MySql]
host=127.0.0.1
username=tester
password={NAe3[]<
dbname=rtalib_db

[Redis]
host=127.0.0.1
password=
dbname=1
indexon=rtalib_dl_test_table:timerealtt,rtalib_test_table:a
EOL

cat rtalibconfig_testing.conf'''
        sh 'SINGULARITYENV_RTALIBCONFIGFILE=./rtalibconfig_testing singularity exec --cleanenv ../images/rtalib-env.sigm env'
      }
    }
    stage('RTAlib testing') {
      steps {
        echo 'RTAlib testing'
      }
    }
    stage('Unit-testing') {
      parallel {
        stage('Unit-testing') {
          steps {
            sh 'singularity exec ../images/rtalib-env.sigm bash PyRTAlib/TestEnvironment/unit_tests/run_unit_tests.sh'
          }
        }
        stage('Test coverage') {
          steps {
            echo 'Coverage test'
          }
        }
      }
    }
  }
}