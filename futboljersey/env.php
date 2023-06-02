<?PHP 

//PAYJPAPI設定
/**
 * 環境にAPI用定数追加
 */
function payjp_init(){
putenv("PAYJP_PUBLIC_KEY=pk_test_e525c2e6ae24765b9e5bf9e1");
putenv("PAYJP_SECRET_KEY=sk_test_6734d57126c3f4218b822b72");
putenv("PAYJP_CURRENCY=jpy");
putenv("PAYJP_CAPTURE_MODE=false");
putenv("PAYJP_EXPIRY_DAYS=60");
}