# Bme280
Récupère les données provenant du capteur BME280 toutes les 5s environ

# bme280_json
Récupère les données provenant du capteur BME280 ainsi que la date et l'heure et les encode au format Json

# bme280_json

## bme280_json_rx
Réception des données du capteur de température par loRa, en format JSON
Décodage JSON et affichage

## bme280_json_tx
A chaque évènement d’interruption, le microcontrôleur récupère les données provenant du capteur BME280 ainsi que la date et l'heure (configurées en « dur » dans le programme), les encode au format Json et les transmet via LoRa.   Problème avec l’interruption !!!

# bme280_rtc
Récupère les données du capteur de température chaque minute (rtc)

# reveil esclave

## esclave1_noeud et esclave_noeud2
Réveil de la carte par SX1272 et réception des données, fonctionne avec reveil_noeud

## reveil_noeud
Réveil esclave1 et esclave2 en même temps malgré la fonction setnode

# gps_seul
Acquisition des données GPS

# gps_bme

## gps_bme_master_simple
Récupère les info gps et bme au format json et les décode

## gps_bme_simple
Acquisition capteur, json, transmission

# gps_bme_ack

## gps_bme_master_simple_ack
Reçoit les info gps et bme au format json et les décode, avec ack

## gps_bme_simple_ack
Acquisition capteur, json, transmission avec ack

# gps_simple
Acquisition gps, transmission lora via le shield

# gps_sanswhile
Similaire à gps_simple mais ne vérifie pas si le gps est connecté

# gps_SLAVE 
Fonctionne avec gps_MASTER, reçoit un ordre du maitre pour lancer la recherche, désactive le gps après une durée TMAX. Ne fonctionne pas

# gps_tmax
Désactive le gps après Tmax, pas de transmission

# lora_config_noshield
Configuration du lora sans le shield

# Master/Slace

## SLAVE
Ce programme reçoit un ordre de MASTER
 Cet ordre permet de déclencher la mesure capteur et de transmettre les infos

## MASTER
Envoi des ordres à SLAVE et reçoit les données

# mode_sleep_m0
Une façon de reveiller la carte, pas de librairie spécifique

# mode_sleep_m0_library_rest
Essai librairie rest.h

# mode_sleep_reception_rtc
Réveil de la carte par rtc

# reception_bme280_json
Réveil de la carte avec librairie RTCZero puis reception des données

# RTCZero

## reception_RTCZero
Réveil carte RTCZero puis reception, fonctionne avec transmission_RTCZero

## transmission_RTCZero
Voir reception_RTCZero

# reveil_sx1272_rxdone
sx1272 reveille l'arduino via rxdone

# test_capteur_temperature_analog
Test de l'arduino avec capteur de température analogique

# test_capteur_temperature_analog_i2c_m0
Capteur i2c et capteur analogique

