pipeline {
  agent any
  stages {
    stage('Get testing environment') {
      steps {
        sh 'singularity exec ../images/rtalib-env.sigm python --version'
      }
    }
  }
}