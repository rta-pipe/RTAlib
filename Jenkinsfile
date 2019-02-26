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
        sh '/usr/local/bin/singularity --version'
        sh 'rm -rf bind_dirs'
        sh 'mkdir -p bind_dirs/lib/mysql && mkdir -p bind_dirs/log && mkdir -p bind_dirs/run/mysqld && mkdir -p bind_dirs/rtalib_jenkins'
        sh 'ls /usr/local/bin/'
        sleep 2
        sh '/usr/local/bin/singularity instance.start --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run --bind /var/lib/jenkins/workspace/RTAlib_jenkins_master:/bind_dirs/rtalib_jenkins --cleanenv ../singularity_images/rta_lib_env_service.sif rta_lib_env_service'
        sleep 2
      }
    }
    stage('PyRTAlib Unit-testing') {
      parallel {
        stage('Unit-testing') {
          steps {
            sh 'rm -rf /home/jenkins/test-reports'
            sh '/usr/local/bin/singularity exec --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run --bind /var/lib/jenkins/workspace/RTAlib_jenkins_master:/bind_dirs/rtalib_jenkins ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/pip install xmlrunner --user jenkins'
            sh '''SINGULARITYENV_RTALIBCONFIG=/bind_dirs/rtalib_jenkins/rtalibconfig_testing /usr/local/bin/singularity exec --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run --bind /var/lib/jenkins/workspace/RTAlib_jenkins_master:/bind_dirs/rtalib_jenkins --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/rtalib_jenkins/PyRTAlib/TestEnvironment/unit_tests/Config_unittest.py

SINGULARITYENV_RTALIBCONFIG=/bind_dirs/rtalib_jenkins/rtalibconfig_testing /usr/local/bin/singularity exec --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run --bind /var/lib/jenkins/workspace/RTAlib_jenkins_master:/bind_dirs/rtalib_jenkins --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/rtalib_jenkins/PyRTAlib/TestEnvironment/unit_tests/MySqlDBConnector_unittest.py

SINGULARITYENV_RTALIBCONFIG=/bind_dirs/rtalib_jenkins/rtalibconfig_testing /usr/local/bin/singularity exec --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run --bind /var/lib/jenkins/workspace/RTAlib_jenkins_master:/bind_dirs/rtalib_jenkins --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/rtalib_jenkins/PyRTAlib/TestEnvironment/unit_tests/RedisDBConnector_unittest.py

SINGULARITYENV_RTALIBCONFIG=/bind_dirs/rtalib_jenkins/rtalibconfig_testing /usr/local/bin/singularity exec --bind bind_dirs/lib:/var/lib --bind bind_dirs/lib/mysql:/var/lib/mysql --bind bind_dirs/log:/var/log --bind bind_dirs/run:/var/run --bind /var/lib/jenkins/workspace/RTAlib_jenkins_master:/bind_dirs/rtalib_jenkins --cleanenv ../singularity_images/rta_lib_env_service.sif /opt/anaconda3/envs/rtalib-env/bin/python /bind_dirs/rtalib_jenkins/PyRTAlib/TestEnvironment/unit_tests/RTA_DL_DB_unittest.py'''
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
    stage('Test reporting') {
      steps {
        sh 'cp -r /home/jenkins/test-reports/ ./'
        junit 'test-reports/*.xml'
      }
    }
  }
}