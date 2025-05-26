import logging

# Logger mit dem Namen 'MyLoger' erstellen
logger = logging.getLogger('MyLogger')
logger.setLevel(logging.DEBUG)  # Loglevel auf DEBUG setzen (zeigt alle Meldungen)

# Konsolen-Handler erstellen und auf DEBUG-Level setzen
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)

# Formatierer erstellen: Zeitstempel, Loggername, Loglevel, Nachricht
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

# Formatierer dem Konsolen-Handler zuweisen
ch.setFormatter(formatter)

# Konsolen-Handler dem Logger hinzufügen
logger.addHandler(ch)

# 'Anwendungscode' – hier werden Log-Meldungen erzeugt
logger.debug('Debug-Nachricht')
logger.info('Info-Nachricht')
logger.warning('Warnung')
logger.error('Fehlermeldung')
logger.critical('Kritische Meldung')
