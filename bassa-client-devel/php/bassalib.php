<?php
/** Copyright 2008 - Rashan Anushka (rashan.uoc@gmail.com)
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with SharpGps; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * Returns a Bassa service object
 *
 * @return Bassa service Object
 */
function getBassaService(){
	global $service,$h,$p;
	if($service==null){
		$service = new SoapClient("bassa.wsdl",array(
		"soap_version"   => SOAP_1_2,
		"trace"      => 1,
		"exceptions" => 1,
		"location" => "http://".$h.":".$p));
	}
	return $service;
}


/** Search downloads for a given search keyword
 * 
 * @param string $token
 */
function search($token,$offset=0,$sorttype=0){
	global $service;
	$service = getBassaService();
	try{
		$results = $service->search($offset,$sorttype,$token);
	}catch(SoapFault $e){
		print($e->faultstring);
		return false;
	}
	bassaDebug();
	if($results['total'] == 0) return false;
	else return $results;
}


/** Add a download for the Bassa
 * 
 * @param string $url
 * @param string $cLength
 * @param string $authToken
 * @param string $uid
 */
function add($url,$cLength,$authToken,$uid){
	global $service;

	$request = new object();
	$request->url = $url;
	$request->{content-length} = $cLength;
	$request->{auth-token-val} = $authToken;
	$request->uuid = $uid;

	$service = getBassaService();
	try{
		$result = $service->enqueue($request);
	}catch(SoapFault $e){
		print($e->faultstring);
		return false;
	}
	bassaDebug();
	return $result;
}


/**
 * List the latest downloads completed by the Bassa
 * 
 * @param int $number
 */
function getLatestDownloads($number=0){
	global $service;
	$service= getBassaService();
	try{
		$results = $service->__soapCall("latest-downloads", array($number));
	}catch(SoapFault $e){
		print($e->faultstring);
		return false;
	}
	bassaDebug();
	if($results['total'] == 0) return false;
	else return $results;
}

/**
 * List all the downloads by the Bassa server
 * 
 */
function getAllDownloads($offset=0,$sorttype=0){
	global $service;
	$client = getBassaService();
	try{
		$results = $client->__soapCall("list-all", array($offset,$sorttype));
	}catch(SoapFault $e){
		print($e->faultstring);
		return false;
	}
	bassaDebug();
	if($results && $results['total'] == 0) return false;
	else return $results;
}

/**
 * List all downloads by a given User
 * 
 * @param string $userid
 */
function getUserDownloads($userid,$offset=0,$sorttype=0){
	global $service;
	$client = getBassaService();
	try{
		$results = $client->__soapCall("list-byuuid", array($offset,$sorttype,$userid));
	}catch(SoapFault $e){
		print($e->faultstring);
		return false;
	}
	bassaDebug();
	if($results && $results['total'] == 0) return false;
	else return $results;
}

/**
 * Get the status of a download request
 * 
 * @param int $id
 */
function getStatus($id){
	global $service;
	$service = getBassaService();
	try{
		$result = $service->__soapCall("get-status", array($id));
	}catch(SoapFault $e){
		print($e->faultstring);
		return false;
	}
	bassaDebug();
	return $result;
}

/**
 * If the debug mode is enabled,Display the last SOAP request and it's Responsee
 */
function bassaDebug(){
	global $service,$debug;
	if(BASSA_DEBUG || $debug){
		print "<pre>";
		print "Request :\n".htmlspecialchars($service->__getLastRequest()) ."\n";
		print "Response:\n".htmlspecialchars($service->__getLastResponse())."\n";
		print "</pre>";
	}
}

/**
 * Print the usage
 */
function printBassaUsage(){
	print "<h4>Usage Parameters</h4>";
	print "<br/><b>Host:</b>  host";
	print "<br/><b>Port:</b>  port";
	print "<br/><b>Action:</b>  action={list,search,latest}";
	print "<br/><b>Search Tocken:</b>  q";
}

/**
 * To view available functions
*/

$service = getBassaService();
foreach ( $service->__getFunctions() as $key1 => $val1) {
        print "$val1";
        print "<br><br>";
}

?>
