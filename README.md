# rac-save-converter
An **experimental** tool to convert Ratchet & Clank saves between PS3 and PSVita.
This requires a Vita with taiHEN and VitaShell (or any shell that can open encrypted folders in clear).
# How to use : 
- ## PS3 to Vita :
 - Copy your save file from PS3 to a USB drive.
 - Decrypt it with Bruteforce Save Data or a similar tool.
 - Use rac-sc to convert your file.
 - Copy the converted save on your Vita, in ux0:data.
 - Open Vitashell.
 -  -  Go to ux0:data
 -  -  Copy your converted file.
 -  - Go to ux0:user/00/savedata
 -  - Highlight the **PCSF00486** folder, press Δ and select "Open Decrypted".
 - - Paste the modified savefile in this folder (be sure you backed up your original save just in case, accessing it with the Δ > "Open Decrypted" *trick*)
 - See if save works.
 
 
- ## Vita to PS3 :
 - Copy a save file of the target game and user from PS3 to a USB drive.
 - Open it with Bruteforce Save Data or a similar tool, so that it has your user info.
 -  - Open Vitashell.
 -  - Go to ux0:user/00/savedata
 - - Highlight your game's save folder, press Δ and select "Open Decrypted".
 - - Copy the file you want to convert to ux0:data.
 - Copy the file to your PC.
 - Convert it with rac-sc.
 - Encrypt your new save file with Bruteforce Save Data or a similar tool.
 - Copy the encrypted file to your USB drive (be sure you backed up your original save just in case)
 - Copy the save on the PS3.

### In case a R&C3 converted save doesn't work, open an issue.

## To-do :
- **Test the RC3 method in-depth** (your help is needed)
- Implement drag-and-drop conversion *(Soon™)*
- Retro-engineer the RC1 and RC2 saves to get a reliable method 
- - (RAC1 method is *experimental*, RAC2 one is **not implemented**)
- - More tests needs to be done to see what gets broken with differents saves


## Credits :
Big shoutouts to :
- @JohnsonJackblue for a sane base code and all the help and tips he gave me.
- isakiBV for the explainations about RAC3 saves.
- everyone else that worked on RE'ing the RAC3 saves.

## Changelog :
- 0.3 : First public release.
