<?php
/**
 * @file
 * This file is included from settings.php in both dev and dh-make-drupal
 * deployed sites.
 * Put any settings that are shared between staging, prod and dev in here
 */

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

// Archimedes email address. When using mail to send reports to archimedes
// server, this will be the address to send the reports too. This should
// be the preferred method as it will work outside of the Catalyst LAN
// and has the capability to get around firewalls that HTTP often cannot.
$conf['archimedes_server_email'] = 'archimedes@catalyst.net.nz';

// Archimedes report sending method. There are two options here; send via
// HTTP (postXML) or via email (sendXML). The default and most reliable
// method is the latter.
$conf['archimedes_send_method'] = 'sendXML';
