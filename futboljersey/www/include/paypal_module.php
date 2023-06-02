<?PHP
/*

	PayPal支払い関数プログラム

	make ookawara 2013/11/19

*/


	//	初期設定
	$PROXY_HOST = '127.0.0.1';
	$PROXY_PORT = '808';

	$SandboxFlag = false;	//	true(Sandbox利用) or false(Sandbox未使用　本番)

	//'------------------------------------
	//' PayPal API Credentials
	//' Replace <API_USERNAME> with your API Username
	//' Replace <API_PASSWORD> with your API Password
	//' Replace <API_SIGNATURE> with your Signature
	//'------------------------------------

	//	テスト用
	$API_UserName="yacchan_api1.azet.jp";
	$API_Password="1391402126";
	$API_Signature="A5BnOCOv6rbWXnO4GI1J8HTsAUPMA1Two8rY2eWDl6CyfeBHOMokXSoD";
	$RETURNURL = "https://www.futboljersey.com/cago.php?m=comp";
	$CANCELURL = "https://www.futboljersey.com/cago.php?m=cancel";
	$THANKSURL = HTTPS."/cago.php?num=thanks";

	//	本番用
	if ($SandboxFlag != true) {
		$API_UserName="query_api1.futboljersey.com";
		$API_Password="JNSK7KHDHZTA5J48";
		$API_Signature="An5ns1Kso7MWUdW4ErQKJJJ4qi4-ADJaBgYy51A5hgcA1QmPL224rhZZ";
		$RETURNURL = "https://www.futboljersey.com/cago.php?m=comp";
		$CANCELURL = "https://www.futboljersey.com/cago.php?m=cancel";
		$THANKSURL = HTTPS."/cago.php?num=thanks";
	}

	define("RETURNURL", $RETURNURL);
	define("CANCELURL", $CANCELURL);
	define("THANKSURL", $THANKSURL);


	// BN Code 	is only applicable for partners
	$sBNCode = "PP-ECWizard";

	/*	
	' Define the PayPal Redirect URLs.  
	' 	This is the URL that the buyer is first sent to do authorize payment with their paypal account
	' 	change the URL depending if you are testing on the sandbox or the live PayPal site
	'
	' For the sandbox, the URL is       https://www.sandbox.paypal.com/webscr&cmd=_express-checkout&token=
	' For the live site, the URL is        https://www.paypal.com/webscr&cmd=_express-checkout&token=
	*/
	
	if ($SandboxFlag == true) {
		$API_Endpoint = "https://api-3t.sandbox.paypal.com/nvp";
		$PAYPAL_URL = "https://www.sandbox.paypal.com/webscr?cmd=_express-checkout&token=";
	} else {
		$API_Endpoint = "https://api-3t.paypal.com/nvp";
		$PAYPAL_URL = "https://www.paypal.com/cgi-bin/webscr?cmd=_express-checkout&token=";
	}

	$USE_PROXY = false;
	$version="93";


/*   
	'-------------------------------------------------------------------------------------------------------------------------------------------
	' Purpose: 	Prepares the parameters for the SetExpressCheckout API Call.
	' Inputs:  
	'		paymentAmount:  	Total value of the shopping cart
	'		currencyCodeType: 	Currency code value the PayPal API
	'		paymentType: 		paymentType has to be one of the following values: Sale or Order or Authorization
	'		returnURL:			the page where buyers return to after they are done with the payment review on PayPal
	'		cancelURL:			the page where buyers return to when they cancel the payment review on PayPal
	'--------------------------------------------------------------------------------------------------------------------------------------------	
*/
function CallShortcutExpressCheckout( $PARAMETER , $ORDER_LIST ) {
	//------------------------------------------------------------------------------------------------------------------------------------
	// Construct the parameter string that describes the SetExpressCheckout API call in the shortcut implementation


	$nvpstr = "";
	$nvpstr = $nvpstr . "&PAYMENTREQUEST_0_PAYMENTACTION=" . $PARAMETER['paymentType'];
	$nvpstr = $nvpstr . "&RETURNURL=" . $PARAMETER['returnURL'];
	$nvpstr = $nvpstr . "&CANCELURL=" . $PARAMETER['cancelURL'];
	$nvpstr = $nvpstr . "&PAYMENTREQUEST_0_CURRENCYCODE=" . $PARAMETER['currencyCodeType'];
	$nvpstr = $nvpstr . "&LOCALECODE=JP";
	
	if ($_SESSION['paypal_list']) {

		$nvpstr = $nvpstr . "&PAYMENTREQUEST_0_AMT=". $ORDER_LIST['TOTAL'];
		$_SESSION["Payment_Amount"] = $ORDER_LIST['TOTAL'];
/*
		//	データーを送信しきれない為利用しない
		if ($ORDER_LIST) {
			foreach ($ORDER_LIST AS $key => $VALUE) {
				if ($key == "TOTAL") { continue; }
				if ($VALUE) {
					foreach ($VALUE AS $key_name => $val) {
						if ($val == "") { continue; }
						$nvpstr = $nvpstr . "&" . $key_name . "=".urlencode($val);
					}
				}
			}
		}
*/
	}

	$total_price = number_format($ORDER_LIST['TOTAL']);
	//$thank_msg = "ご注文ありがとうございます。商品ご購入代金、".$total_price."円(消費税・送料・手数料など含む)の支払い手続きをお願い致します。";
	$thank_msg = "ご購入代金、".$total_price."円(消費税・送料・手数料など含む)の支払い手続きをお願い致します。";
	$nvpstr = $nvpstr . "&PAYMENTREQUEST_0_DESC=" . urlencode($thank_msg);


	$_SESSION["currencyCodeType"] = $PARAMETER['currencyCodeType'];
	$_SESSION["PaymentType"] = $PARAMETER['paymentType'];

	//'--------------------------------------------------------------------------------------------------------------- 
	//' Make the API call to PayPal
	//' If the API call succeded, then redirect the buyer to PayPal to begin to authorize payment.  
	//' If an error occured, show the resulting errors
	//'---------------------------------------------------------------------------------------------------------------
    $resArray=hash_call("SetExpressCheckout", $nvpstr);
	$ack = strtoupper($resArray["ACK"]);
	if($ack=="SUCCESS" || $ack=="SUCCESSWITHWARNING")
	{
		$token = urldecode($resArray["TOKEN"]);
		$_SESSION['TOKEN']=$token;
	}
	   
    return $resArray;
}


/*
	'-------------------------------------------------------------------------------------------------------------------------------------------
	* hash_call: Function to perform the API call to PayPal using API signature
	* @methodName is name of API  method.
	* @nvpStr is nvp string.
	* returns an associtive array containing the response from the server.
	'-------------------------------------------------------------------------------------------------------------------------------------------
*/
function hash_call($methodName,$nvpStr) {
	//declaring of global variables
	global $API_Endpoint, $version, $API_UserName, $API_Password, $API_Signature;
	global $USE_PROXY, $PROXY_HOST, $PROXY_PORT;
	global $gv_ApiErrorURL;
	global $sBNCode;

	//setting the curl parameters.
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,$API_Endpoint);
	curl_setopt($ch, CURLOPT_VERBOSE, 1);

	//turning off the server and peer verification(TrustManager Concept).
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
	curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);

	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch, CURLOPT_POST, 1);
	
    //if USE_PROXY constant set to TRUE in Constants.php, then only proxy will be enabled.
   //Set proxy name to PROXY_HOST and port number to PROXY_PORT in constants.php 
	if($USE_PROXY)
		curl_setopt ($ch, CURLOPT_PROXY, $PROXY_HOST. ":" . $PROXY_PORT); 

	//NVPRequest for submitting to server
	$nvpreq = "METHOD=" . urlencode($methodName) .
			  "&VERSION=" . urlencode($version) .
			  "&PWD=" . urlencode($API_Password) .
			  "&USER=" . urlencode($API_UserName) .
			  "&SIGNATURE=" . urlencode($API_Signature) . $nvpStr .
			  "&BUTTONSOURCE=" . urlencode($sBNCode);

	//setting the nvpreq as POST FIELD to curl
	curl_setopt($ch, CURLOPT_POSTFIELDS, $nvpreq);

	//getting response from server
	$response = curl_exec($ch);

	//convrting NVPResponse to an Associative Array
	$nvpResArray=deformatNVP($response);
	$nvpReqArray=deformatNVP($nvpreq);
	$_SESSION['nvpReqArray']=$nvpReqArray;

	if (curl_errno($ch)) {
		// moving to display page to display curl errors
		  $_SESSION['curl_error_no']=curl_errno($ch) ;
		  $_SESSION['curl_error_msg']=curl_error($ch);

		  //Execute the Error handling module to display errors. 
	} else {
		 //closing the curl
	  	curl_close($ch);
	}

	return $nvpResArray;
}


/*
	'----------------------------------------------------------------------------------
	 Purpose: Redirects to PayPal.com site.
	 Inputs:  NVP string.
	 Returns: 
	----------------------------------------------------------------------------------
*/
function RedirectToPayPal ( $token ) {
	global $PAYPAL_URL;

	// Redirect to paypal.com here
	$payPalURL = $PAYPAL_URL . $token;
	header("Location: ".$payPalURL);
	exit;
}


/*
	'----------------------------------------------------------------------------------
	* This function will take NVPString and convert it to an Associative Array and it will decode the response.
	* It is usefull to search for a particular key and displaying arrays.
	* @nvpstr is NVPString.
	* @nvpArray is Associative Array.
	'----------------------------------------------------------------------------------
 */
function deformatNVP($nvpstr) {
	$intial=0;
 	$nvpArray = array();

	while(strlen($nvpstr)) {
		//postion of Key
		$keypos= strpos($nvpstr,'=');
		//position of value
		$valuepos = strpos($nvpstr,'&') ? strpos($nvpstr,'&'): strlen($nvpstr);

		/*getting the Key and Value values and storing in a Associative Array*/
		$keyval=substr($nvpstr,$intial,$keypos);
		$valval=substr($nvpstr,$keypos+1,$valuepos-$keypos-1);
		//decoding the respose
		$nvpArray[urldecode($keyval)] =urldecode( $valval);
		$nvpstr=substr($nvpstr,$valuepos+1,strlen($nvpstr));
     }

	return $nvpArray;
}


	/*
	'-------------------------------------------------------------------------------------------
	' Purpose: 	Prepares the parameters for the GetExpressCheckoutDetails API Call.
	'
	' Inputs:  
	'		None
	' Returns: 
	'		The NVP Collection object of the GetExpressCheckoutDetails Call Response.
	'-------------------------------------------------------------------------------------------
	*/
	function GetShippingDetails( $token )
	{
		//'--------------------------------------------------------------
		//' At this point, the buyer has completed authorizing the payment
		//' at PayPal.  The function will call PayPal to obtain the details
		//' of the authorization, incuding any shipping information of the
		//' buyer.  Remember, the authorization is not a completed transaction
		//' at this state - the buyer still needs an additional step to finalize
		//' the transaction
		//'--------------------------------------------------------------
	   
	    //'---------------------------------------------------------------------------
		//' Build a second API request to PayPal, using the token as the
		//'  ID to get the details on the payment authorization
		//'---------------------------------------------------------------------------
	    $nvpstr="&TOKEN=" . $token;

		//'---------------------------------------------------------------------------
		//' Make the API call and store the results in an array.  
		//'	If the call was a success, show the authorization details, and provide
		//' 	an action to complete the payment.  
		//'	If failed, show the error
		//'---------------------------------------------------------------------------
	    $resArray=hash_call("GetExpressCheckoutDetails",$nvpstr);
	    $ack = strtoupper($resArray["ACK"]);
		if($ack == "SUCCESS" || $ack=="SUCCESSWITHWARNING")
		{	
			$_SESSION['payer_id'] =	$resArray['PAYERID'];
		} 
		return $resArray;
	}




//	PayPal支払い手続き
//	add ookawara 2013/11/15
//	ookawara 2014/01/24 /include/cago.phpから移動
function paypal(&$ERROR) {

	//	初期設定
	$PARAMETER = array();
	$PARAMETER['currencyCodeType'] = "JPY";
	$PARAMETER['paymentType'] = "Authorization";
	$PARAMETER['returnURL'] = RETURNURL;
	$PARAMETER['cancelURL'] = CANCELURL;

	$ORDER_LIST = array();
	if ($_SESSION['paypal_list']) {
		$ORDER_LIST = $_SESSION['paypal_list'];
	}

	$resArray = CallShortcutExpressCheckout ( $PARAMETER , $ORDER_LIST );
	$ack = strtoupper($resArray["ACK"]);
	if($ack=="SUCCESS" || $ack=="SUCCESSWITHWARNING") {
		RedirectToPayPal ( $resArray["TOKEN"] );
	} else {
		//Display a user friendly Error on the page using any of the following error information returned by PayPal
		$ErrorCode = urldecode($resArray["L_ERRORCODE0"]);
		$ErrorShortMsg = urldecode($resArray["L_SHORTMESSAGE0"]);
		$ErrorLongMsg = urldecode($resArray["L_LONGMESSAGE0"]);
		$ErrorSeverityCode = urldecode($resArray["L_SEVERITYCODE0"]);
		
		$html .= "SetExpressCheckout API call failed. ";
		$html .= "Detailed Error Message: " . $ErrorLongMsg;
		$html .= "Short Error Message: " . $ErrorShortMsg;
		$html .= "Error Code: " . $ErrorCode;
		$html .= "Error Severity Code: " . $ErrorSeverityCode;

		$ERROR[] = $html;
	}

}



//	paypal支払い後確認
//	add ookawara 2014/01/24
function paypal_comp_check(&$ERROR) {

	$token = $_REQUEST['token'];

	$resArray = GetShippingDetails( $token );
	$ack = strtoupper($resArray["ACK"]);

	if( $ack != "SUCCESS" && $ack != "SUCESSWITHWARNING") {
		$ERROR[] = "支払い処理が確認出来ませんでした。";

		$ErrorCode = urldecode($resArray["L_ERRORCODE0"]);
		$ErrorShortMsg = urldecode($resArray["L_SHORTMESSAGE0"]);
		$ErrorLongMsg = urldecode($resArray["L_LONGMESSAGE0"]);
		$ErrorSeverityCode = urldecode($resArray["L_SEVERITYCODE0"]);

		$ERROR[] = "Detailed Error Message: " . $ErrorLongMsg;
		$ERROR[] = "Short Error Message: " . $ErrorShortMsg;
		$ERROR[] = "Error Code: " . $ErrorCode;
		$ERROR[] = "Error Severity Code: " . $ErrorSeverityCode;
	}

}


//	最終支払いセット
function last_set_paypal() {

	$resArray_last = "";

	$finalPaymentAmount =  $_SESSION["Payment_Amount"];

	$resArray = ConfirmPayment ( $finalPaymentAmount );
	$ack = strtoupper($resArray["ACK"]);
	if( $ack == "SUCCESS" || $ack == "SUCCESSWITHWARNING" ) {
		$token		= $resArray["TOKEN"];
		$transactionId		= $resArray["PAYMENTINFO_0_TRANSACTIONID"];
		$transactionType 	= $resArray["TRANSACTIONTYPE"];
		$paymentType		= $resArray["PAYMENTTYPE"];
		$orderTime 			= $resArray["ORDERTIME"];
		$amt				= $resArray["PAYMENTINFO_0_AMT"];
		$currencyCode		= $resArray["CURRENCYCODE"];
		$feeAmt				= $resArray["FEEAMT"];
		$settleAmt			= $resArray["SETTLEAMT"];
		$taxAmt				= $resArray["TAXAMT"];
		$exchangeRate		= $resArray["EXCHANGERATE"];

		$paymentStatus	= $resArray["PAYMENTSTATUS"];
		$pendingReason	= $resArray["PENDINGREASON"];
		$reasonCode		= $resArray["REASONCODE"];

	} else {
#		$ERROR[] = mb_convert_encoding("<font style='font-size:20px;color:#ff0000;font-weight:bold;'>Paypalによる支払いを行う事が出来ませんでした。<br />　別のカードで再度ご注文下さい。</font>", "EUC-JP", "UTF-8");
		$ERROR[] = "<font style='font-size:20px;color:#ff0000;font-weight:bold;'>Paypalによる支払いを行う事が出来ませんでした。<br />　別のカードで再度ご注文下さい。</font>";
		$_SESSION['PAYPAL_ERROR'] = $ERROR;
		$url = CANCELURL;
		header ("Location: $url\n\n");
		exit;
	}

}
	/*
	'-------------------------------------------------------------------------------------------------------------------------------------------
	' Purpose: 	Prepares the parameters for the GetExpressCheckoutDetails API Call.
	'
	' Inputs:  
	'		sBNCode:	The BN code used by PayPal to track the transactions from a given shopping cart.
	' Returns: 
	'		The NVP Collection object of the GetExpressCheckoutDetails Call Response.
	'--------------------------------------------------------------------------------------------------------------------------------------------	
	*/
	function ConfirmPayment( $FinalPaymentAmt )
	{
		/* Gather the information to make the final call to
		   finalize the PayPal payment.  The variable nvpstr
		   holds the name value pairs
		   */
		

		//Format the other parameters that were stored in the session from the previous calls	
		$token 				= urlencode($_SESSION['TOKEN']);
		$paymentType 		= urlencode($_SESSION['PaymentType']);
		$currencyCodeType 	= urlencode($_SESSION['currencyCodeType']);
		$payerID 			= urlencode($_SESSION['payer_id']);

		$serverName 		= urlencode($_SERVER['SERVER_NAME']);

		$nvpstr  = '&TOKEN=' . $token . '&PAYERID=' . $payerID . '&PAYMENTREQUEST_0_PAYMENTACTION=' . $paymentType . '&PAYMENTREQUEST_0_AMT=' . $FinalPaymentAmt;
		$nvpstr .= '&PAYMENTREQUEST_0_CURRENCYCODE=' . $currencyCodeType . '&IPADDRESS=' . $serverName; 

		 /* Make the call to PayPal to finalize payment
		    If an error occured, show the resulting errors
		    */
		$resArray=hash_call("DoExpressCheckoutPayment",$nvpstr);

		/* Display the API response back to the browser.
		   If the response from PayPal was a success, display the response parameters'
		   If the response was an error, display the errors received using APIError.php.
		   */
		$ack = strtoupper($resArray["ACK"]);

		return $resArray;
	}


	function DoReferenceTransaction($paymentAmt,$currencyId,$transactionId)
	{
		//'--------------------------------------------------------------
		//' At this point, the buyer has completed authorizing the payment
		//' at PayPal.  The function will call PayPal to obtain the details
		//' of the authorization, incuding any shipping information of the
		//' buyer.  Remember, the authorization is not a completed transaction
		//' at this state - the buyer still needs an additional step to finalize
		//' the transaction
		//'--------------------------------------------------------------

	    //'---------------------------------------------------------------------------
		//' Build a second API request to PayPal, using the token as the
		//'  ID to get the details on the payment authorization
		//'---------------------------------------------------------------------------
	    $nvpstr ="&REFERENCEID=".$transactionId;
	    $nvpstr.="&PAYMENTACTION=Authorization";
	    $nvpstr.="&AMT=".$paymentAmt;
	    $nvpstr.="&CURRENCYCODE=".$currencyId;

		//'---------------------------------------------------------------------------
		//' Make the API call and store the results in an array.  
		//'	If the call was a success, show the authorization details, and provide
		//' 	an action to complete the payment.  
		//'	If failed, show the error
		//'---------------------------------------------------------------------------
	    $resArray=hash_call("DoReferenceTransaction",$nvpstr);
	    $ack = strtoupper($resArray["ACK"]);
		if($ack == "SUCCESS" || $ack=="SUCCESSWITHWARNING")
		{	
			$_SESSION['payer_id'] =	$resArray['PAYERID'];
		} 
		return $resArray;
	}

?>