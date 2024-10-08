<?php

/*
 * Copyright (C) 2019 Deciso B.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

function opendns_configure()
{
    return [
        'bootup' => ['opendns_configure_do'],
        'local' => ['opendns_configure_do'],
        'newwanip_map' => ['opendns_configure_do'],
    ];
}

function opendns_configure_do($verbose = false)
{
    global $config;

    if (!empty($config['opendns']['enable'])) {
        service_log('Configure OpenDNS...', $verbose);

        $result = opendns_register($config['opendns']);
        log_msg("opendns response: $result");

        service_log("done.\n", $verbose);
    }
}

function opendns_xmlrpc_sync()
{
    return [[
        'description' => gettext('OpenDNS'),
        'section' => 'opendns',
        'id' => 'opendns',
    ]];
}

function opendns_register($pconfig)
{
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, sprintf('https://updates.opendns.com/nic/update?hostname=%s', $pconfig['host']));
    curl_setopt($ch, CURLOPT_USERPWD, sprintf('%s:%s', $pconfig['username'], $pconfig['password']));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $output = curl_exec($ch);
    curl_close($ch);

    return $output;
}
