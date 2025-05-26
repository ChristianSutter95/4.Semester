import logging
import sys

# Kommandozeile nach Argument --log=... durchsuchen
for arg in sys.argv:
    if arg.startswith('--log='):
        # Extrahiere den Wert nach dem '='
        loglevel = arg.split('=')[1]

# Versuche, den übergebenen Loglevel-Namen in den entsprechenden Zahlenwert zu übersetzen
# z. B. "INFO" -> logging.INFO (entspricht dem Zahlenwert 20)
numeric_level = getattr(logging, loglevel.upper(), None)

# Fehlerbehandlung: ist das Ergebnis gültig?
if not isinstance(numeric_level, int):
    raise ValueError(f'Ungültiger Log-Level: {loglevel}')

# Logging konfigurieren
logging.basicConfig(
    level=numeric_level,
    format='%(levelname)s: %(message)s'
)


logging.debug('Dies ist eine Debug-Nachricht')
logging.info('Dies ist eine Info-Nachricht')
logging.warning('Dies ist eine Warnung')
logging.error('Dies ist eine Fehlermeldung')
logging.critical('Dies ist eine kritische Meldung')
