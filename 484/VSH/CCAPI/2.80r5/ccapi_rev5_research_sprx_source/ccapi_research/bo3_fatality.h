







void write_payload(uint32_t start_addr, const char *file);

void inject_bo3_fatality()
{
   write_payload(BO3_FATALITY_START_ADR, "/dev_usb000/plugins/fatality_by_enstone.bin");
}