from .DTR import DTR

class DTRtimeVSenergy(DTR):
    def __init__(self, configFilePath=''):
        super().__init__(configFilePath)
        self.outputchannel = self.config.get('Dtr','outputchannel')+'.timevsenergy'

    def transformData(self, data):
        return {'energy': data['energy'], 'time': data['timerealtt'], 'isUpperLimit': True if data['energy']<= 2.9 else False }
