<html>
<head>
<title>BrmHeim - LEDs and audio controlled tunnel</title>
<link type="text/css" rel="stylesheet" href="/static/markdown4.css">
</head>
<body>

# BrmHeim - LEDs and audio controlled tunnel

Welcome to API server of BrmHeim project, funded by [NTNU (NO)](https://www.ntnu.no), [TEKS (NO)](http://teks.no), [Meta.Morf (NO)](http://www.metamorf.no), [CIANT (CZ)](http://www.ciant.cz), done in cooperation of hackerspaces [Brmlab, Prague (CZ)](https://brmlab.cz) and [HackHeim, Trondheim (NO)](http://hackheim.no)

## What ? (Project description)

In tunnel below the road, next to Adresseavien, in the [AdressaParken](https://www.ntnu.no/parken), we've added LED strips on the ceiling and connected them with audio/noise level detection, to visualise the environment and let people interact closely with the park itself.

## Where ?
<iframe src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d935.0768662560693!2d10.405310611716255!3d63.43251936413615!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!3m3!1m2!1s0x0%3A0x0!2zNjPCsDI1JzU3LjEiTiAxMMKwMjQnMjMuNiJF!5e1!3m2!1sen!2sno!4v1460809956894" width="600" height="450" frameborder="0" style="border:0" allowfullscreen></iframe>

## JSON read-only API available

  - Noise Level data gathering
    - Use [`/volume`](/volume) to get up to 1000 of volume samples, and list of volume measuring units
    - To get more specific data, from either single client or from all clients in specific time-span use URL templates below
      - [`/volume/CLIENT_ID`](/volume/1)
      - [`/volume/CLIENT_ID/DATETIME_FROM/DATETTIME_TO`](/volume/1/2015-01-01-00-00-00/2016-12-31-00-00-00)
      - [`/volume/CLIENT_ID/DATE_FROM/DATE_TO`](/volume/1/2015-01-01/2016-12-31)
      - [`/volume/CLIENT_ID/DATE_FROM/DATETIME_TO`](/volume/1/2015-01-01/2016-12-31-00-00-00)
      - [`/volume/CLIENT_ID/DATETIME_FROM/DATE_TO`](/volume/1/2015-01-01-00-00-00/2016-12-31)
      - [`/volume/DATETIME_FROM/DATETTIME_TO`](/volume/2015-01-01-00-00-00/2016-12-31-00-00-00)
      - [`/volume/DATE_FROM/DATE_TO`](/volume/2015-01-01/2016-12-31)
      - [`/volume/DATE_FROM/DATETIME_TO`](/volume/2015-01-01/2016-12-31-00-00-00)
      - [`/volume/DATETIME_FROM/DATE_TO`](/volume/2015-01-01-00-00-00/2016-12-31)
   - `CLIENT_ID` is id of client, currently there is only one, with CLIENT_ID=1
   - `DATE_FROM`/`DATE_TO` is in format `YYYY-MM-DD` (month and day is zero prefixed, year in full form), ie. `2016-04-16`
   - `DATETIME_FROM`/`DATETIME_TO` is in format `YYYY-MM-DD-HH-mm-ss` (time is also zero prefixed, in 24-hour format), ie. `2016-04-16-13-30-00`

## Returned JSON data format

### Client

```json
{
CLIENT_ID,
LEDS_COUNT,
LAST_SEEN
}
```

### Volume

```json
{
VOLUME_ID,
VOLUME,
DATE+TIME,
CLIENT_ID
}
```

### Whole JSON response

```json
{
clients: [ ... Client objects ...],
volume: [ ... Volume objects ...]
}
```

</body>
</html>
