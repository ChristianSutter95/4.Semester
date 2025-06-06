import logging

# Logger erstellen
logger = logging.getLogger(__name__)

# Logging-Konfiguration: Ausgabe in Datei 'example.log'
logging.basicConfig(
    filename='example.log',         # Log-Datei
    filemode="w",
    encoding='utf-8',               # Zeichencodierung
    level=logging.DEBUG             # Alle Nachrichten ab DEBUG-Level
)

# Log-Nachrichten
logger.debug('This message should go to the log file')
logger.info('So should this')
logger.warning('And this, too')
logger.error('And non-ASCII stuff, too, like Øresund and Malmö')
