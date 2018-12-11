pipeline {
  agent any
  stages {
    stage('Get testing environment') {
      steps {
        sh 'singularity exec rtalib-env.sigm python --version'
      }
    }
  }
}