pipeline {
  agent any
  stages {
    stage('Get testing environment') {
      steps {
        echo 'RTAlib Jenkins testing pipeline'
        sh 'pwd'
      }
    }
    stage('Creating config file') {
      steps {
        sh '''cat >rtalibconfig_testing <<EOL
[General]
modelname=rtalib_dl_test_table
mjdref=5000
debug=yes
batchsize=1
numberofthreads=1

[Dtr]
active=no
debug=no
inputchannel=

[MySql]
host=127.0.0.1
port=60307
username=tester
password=SingMysqlTester2018
dbname=rtalib_db_test

[Redis]
host=127.0.0.1
port=63800
password=
dbname=1
indexon=rtalib_dl_test_table:timerealtt,rtalib_test_table:a
EOL

cat rtalibconfig_testing'''
      }
    }
    stage('Singularity Instance ') {
      steps {
        echo 'Running the singularity container for the rtalib environment'
        sh 'mkdir -p bind_dirs/lib/mysql && mkdir -p bind_dirs/log && mkdir -p bind_dirs/run/mysqld && mkdir -p bind_dirs/jenkins_rtalib'
        sh 'singularity instance start --bind /var/lib:bind_dirs/lib --bind /var/lib/mysql:bind_dirs/lib/mysql --bind /var/log:bind_dirs/log --bind /var/run:bind_dirs/run --bind /var/lib/jenkins/workspace/RTAlib_master:bind_dirs/jenkins_rtalib ../singularity_images/rta_lib_env_service.sif rta_lib_env_service'
      }
    }
    stage('PyRTAlib Unit-testing') {
      parallel {
        stage('Unit-testing') {
          steps {
            sh '''SINGULARITYENV_RTALIBCONFIG=/bind_dirs/jenkins_rtalib/rtalibconfig_testing singularity exec --bind /var/lib:bind_dirs/lib --bind /var/lib/mysql:bind_dirs/lib/mysql --bind /var/log:bind_dirs/log --bind /var/run:bind_dirs/run --bind /var/lib/jenkins/workspace/RTAlib_master:bind_dirs/jenkins_rtalib --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/jenkins_rtalib/PyRTAlib/TestEnvironment/unit_tests/Config_unittest.py -v

SINGULARITYENV_RTALIBCONFIG=/bind_dirs/jenkins_rtalib/rtalibconfig_testing singularity exec --bind /var/lib:bind_dirs/lib --bind /var/lib/mysql:bind_dirs/lib/mysql --bind /var/log:bind_dirs/log --bind /var/run:bind_dirs/run --bind /var/lib/jenkins/workspace/RTAlib_master:bind_dirs/jenkins_rtalib --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/jenkins_rtalib/PyRTAlib/TestEnvironment/unit_tests/MySqlDBConnector_unittest.py -v

SINGULARITYENV_RTALIBCONFIG=/bind_dirs/jenkins_rtalib/rtalibconfig_testing singularity exec --bind /var/lib:bind_dirs/lib --bind /var/lib/mysql:bind_dirs/lib/mysql --bind /var/log:bind_dirs/log --bind /var/run:bind_dirs/run --bind /var/lib/jenkins/workspace/RTAlib_master:bind_dirs/jenkins_rtalib --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/jenkins_rtalib/PyRTAlib/TestEnvironment/unit_tests/RedisDBConnector_unittest.py -v

SINGULARITYENV_RTALIBCONFIG=/bind_dirs/jenkins_rtalib/rtalibconfig_testing singularity exec --bind /var/lib:bind_dirs/lib --bind /var/lib/mysql:bind_dirs/lib/mysql --bind /var/log:bind_dirs/log --bind /var/run:bind_dirs/run --bind /var/lib/jenkins/workspace/RTAlib_master:bind_dirs/jenkins_rtalib --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/jenkins_rtalib/PyRTAlib/TestEnvironment/unit_tests/RTA_DL_DB_unittest.py -v'''
          }
        }
        stage('PyRTAlib Test coverage') {
          steps {
            echo 'Coverage test'
            sh 'export RTALIBCONFIG=/var/jenkins_home/workspace/RTAlib_master/rtalibconfig_testing'
          }
        }
      }
    }
  }
}