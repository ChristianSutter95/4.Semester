import logging

# Logging-Konfiguration setzen (vor dem ersten Log-Aufruf!)
logging.basicConfig(
    format='%(levelname)s:%(asctime)s %(message)s',  # Format: Level:Zeit Nachricht
    datefmt='%Y-%m-%d %H:%M:%S',                     # Zeitformat
    level=logging.DEBUG                              # Alle Meldungen ab DEBUG anzeigen
)

# Logger mit Namen "MyLogger" erstellen
logger = logging.getLogger('MyLogger')

# Beispielhafte Variablen
benutzer = 'Alice'
aktion = 'Login'
status = 'erfolgreich'

# Logging-Aufrufe mit Variablen
logger.debug('Debugging für Benutzer %s', benutzer)
logger.info('Benutzer %s hat Aktion %s durchgeführt', benutzer, aktion)
logger.warning('Ungewöhnliche Aktivität bei %s', benutzer)
logger.error('Fehler bei Aktion: %s', aktion)
logger.critical('Kritischer Zustand: %s', status)
