import logging

# Filterfunktion: Nur DEBUG-Nachrichten durchlassen
def show_only_debug(record):
    return record.levelname == "DEBUG"

# Logger erstellen und Loglevel auf DEBUG setzen
logger = logging.getLogger(__name__)  # Logger mit Modulnamen
logger.setLevel("DEBUG")              # Akzeptiert alle Level ab DEBUG

# Formatter definieren (für Konsole und Datei)
formatter = logging.Formatter("{levelname} - {message}", style="{")

# Konsole: nur DEBUG-Nachrichten anzeigen
console_handler = logging.StreamHandler()      # Handler für die Konsole
console_handler.setLevel("DEBUG")              # Level setzen (alle ab DEBUG)
console_handler.setFormatter(formatter)        # Format setzen
console_handler.addFilter(show_only_debug)     # Nur DEBUG durchlassen
logger.addHandler(console_handler)             # Handler dem Logger hinzufügen

# Datei: WARNUNG und höher speichern
file_handler = logging.FileHandler("app.log", mode="a", encoding="utf-8")
file_handler.setLevel("WARNING")               # Nur WARNING, ERROR, CRITICAL
file_handler.setFormatter(formatter)           # Gleiches Format wie Konsole
logger.addHandler(file_handler)                # Handler hinzufügen

logger.debug("Just checking in!")      # Geht in Konsole (DEBUG), nicht in Datei
logger.warning("Stay curious!")        # Geht in Datei (WARNING), nicht in Konsole
logger.error("Stay put!")              # Geht in Datei (ERROR), nicht in Konsole
