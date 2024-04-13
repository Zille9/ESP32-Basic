//################################################## Systemparameter anzeigen ###########################################################
    void show_systemparameters(void) {
      Terminal.println();
      Terminal.write("BuiltTime : ");
      Terminal.write(BuiltTime);
      Terminal.println();
      Terminal.write("Release   : ");
      Terminal.write(BasicVersion);
      Terminal.println();
      Terminal.write("Keyboard  : ");
      Terminal.print(Keyboard_lang, DEC);
      Terminal.write("=");
      Terminal.write(Keylayout[Keyboard_lang]);
      Terminal.println();
      Terminal.write("Eeprom-Adr: #");
      Terminal.print(EEPROM.read(11), HEX);
      Terminal.println();
      Terminal.write("SPI-RAM   : ");
      Terminal.print(SPI_RAM_SIZE * 128);
      Terminal.write(" kB");
      Terminal.println();
      Terminal.write("Workpath  : ");
      Terminal.print(sd_pfad);
      line_terminator();
      printmsg("OK>", 0);
    }
