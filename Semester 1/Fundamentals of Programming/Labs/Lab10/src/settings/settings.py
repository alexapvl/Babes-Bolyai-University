import configparser

class GetSettings:
    @staticmethod
    def get_settings() -> str:
        config = configparser.ConfigParser()
        config.read("../settings/settings.properties")
        open_mode = config.get("Settings", "game_mode")
        return open_mode
