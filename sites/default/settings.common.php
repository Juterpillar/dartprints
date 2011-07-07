<?php

// This file is included from settings.php in both dev and dh-make-drupal deployed sites
// Put any settings that are shared between staging, prod and dev in here

// Archimedes server public SSL key. Required by the archimedes module to
// encrypt data. This is done automagically with our debian packaging so
// this is here just for development purposes.
$conf['archimedes_server_key'] = "-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDsqgmiokPIN1nYeUQyMgC/bBUo
3Kba9Swn4Srm+as8nuZZaN1f7kO6yeNFNGF/2s1aWD0dF/7YIJ8kkdy46rodXcQM
7BouZ6owmFYFyvgisbSVOp+4WkdoBg3bwo3yl3jC7ObDtG8tLd5kJv1rkHOcf1vF
8k1s+drkUS5erM+jdQIDAQAB
-----END PUBLIC KEY-----";

// Archimedes URL. When using HTTP to post reports to the archimedes server,
// this will be the URL to post to. As this is an internal URL, this is only
// applicable to development environments within the Catalyst LAN.
$conf['archimedes_server_url'] = "https://archimedes.wgtn.cat-it.co.nz/archimedes-server/update";
