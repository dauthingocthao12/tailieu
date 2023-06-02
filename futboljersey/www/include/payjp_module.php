<?php

require_once(__DIR__ . '/../vendor/payjp/init.php');
require_once(__DIR__ . "/../../env.php");
define("CANCEL_PATH", "/cago.php?m=cancel");

// $inProduction = false;
// if ($inProduction){
//     $PAYJP_API = array(
//         "public_key" => "",
//         "secret_key" => "",
//         "currency" => "jpy",
//         "capture" => false,
//     );
//     $PAYJP_UI_EUC = array(
//         "language" => "jp",
//         "name_placeholder" => "TARO YAMADA",
//         "button_text" => "カードで支払う",
//         "submit_text" => "支払",
//         "script_source" => "https://checkout.pay.jp/",
//         "data_key" => $PAYJP_API["public_key"],
//     );
// } else{
//     $PAYJP_API = array(
//         "public_key" => "pk_test_e525c2e6ae24765b9e5bf9e1",
//         "secret_key" => "sk_test_6734d57126c3f4218b822b72",
//         "currency" => "jpy",
//         "capture" => false,
//     );
//     $PAYJP_UI_EUC = array(
//         "language" => "jp",
//         "name_placeholder" => "TARO YAMADA",
//         "button_text" => "カードで支払う",
//         "submit_text" => "支払",
//         "script_source" => "https://checkout.pay.jp/",
//         "data_key" => $PAYJP_API["public_key"],
//     );
// }
payjp_init();

$PAYJP_API = array(
    "public_key" => getenv("PAYJP_PUBLIC_KEY"),
    "secret_key" => getenv("PAYJP_SECRET_KEY"),
    "currency" => getenv("PAYJP_CURRENCY"),
    "capture" => getenv("PAYJP_CAPTURE_MODE"),
    "expiry_days"=> getenv("PAYJP_EXPIRY_DAYS"),
);
$PAYJP_UI_EUC = array(
    "language" => "ja",
    "name_placeholder" => "TARO YAMADA",
    "button_text" => "カードで支払う",
    "submit_text" => "支払",
    "script_source" => "https://checkout.pay.jp/",
    "data_key" => $PAYJP_API["public_key"],
);
const API_INVALID_REQUEST_ERROR = "決済ができませんでした（コード：101）";
const API_AUTHENTICATION_ERROR = "決済ができませんでした（コード：102）";
const API_CONNECTION_ERROR = "決済ができませんでした（コード：103）";
const API_BASE_ERROR = "決済ができませんでした（コード：104）";
const ERROR = "決済ができませんでした（コード：105）";

$ERROR_PREFIX = "決済ができませんでした（コード: ";
$ERROR_SUFFIX = ")。";

// @link https://pay.jp/docs/api/#error PAYJP API エラーコード
$API_ERROR_CODES = array(
    // "invalid_number" => "	不正なカード番号", //[deprecated]
    // "invalid_cvc" => "	不正なCVC", //[deprecated]
    // "invalid_expiration_date" => "	不正な有効期限年、または月",// [deprecated]
    "incorrect_card_data" => 1, //いずれかのカード情報が誤っている", // [2020/12/10以降新設]
    "invalid_expiry_month" => 2, //不正な有効期限月",
    "invalid_expiry_year" => 3, //不正な有効期限年",
    "expired_card" => 4, //支払いを作成APIで引数customer(とcard)を指定したが紐づくカードが有効期限切れ",
    "card_declined" => 5, //カード会社によって拒否されたカード",
    "card_flagged" => 6, //カードを原因としたエラーが続いたことによる一時的なロックアウト",
    "processing_error" => 8, //決済ネットワーク上で生じたエラー",
    "missing_card" => 9, //支払いを作成APIで指定した引数customerがデフォルトカードを持っていない",
    "unacceptable_brand" => 10, //対象のカードブランドが許可されていない",
    "invalid_id" => 11, //不正なID",
    "no_api_key" => 12, //APIキーがセットされていない",
    "invalid_api_key" => 13, //不正なAPIキー",
    "invalid_plan" => 14, //不正なプラン",
    "invalid_expiry_days" => 15, //不正な失効日数",
    "unnecessary_expiry_days" => 16, //失効日数が不要なパラメーターである",
    "invalid_flexible_id" => 17, //Planなど任意に指定できるIDに対して、命名ルールに違反",
    "invalid_timestamp" => 18, //不正なUnixタイムスタンプ",
    "invalid_trial_end" => 19, //不正なトライアル終了日",
    "invalid_string_length" => 20, //不正な文字列長",
    "invalid_country" => 21, //不正な国名コード",
    "invalid_currency" => 22, //不正な通貨コード",
    "invalid_address_zip" => 23, //不正な郵便番号",
    "invalid_amount" => 24, //不正な支払い金額",
    "invalid_plan_amount" => 25, //不正なプラン金額",
    "invalid_card" => 26, //不正なカード",
    "invalid_card_name" => 27, //不正なカードホルダー名",
    "invalid_card_country" => 28, //不正なカード請求先国名コード",
    "invalid_card_address_zip" => 29, //不正なカード請求先住所(郵便番号)",
    "invalid_card_address_state" => 30, //不正なカード請求先住所(都道府県)",
    "invalid_card_address_city" => 31, //不正なカード請求先住所(市区町村)",
    "invalid_card_address_line" => 32, //不正なカード請求先住所(番地など)",
    "invalid_customer" => 33, //不正な顧客",
    "invalid_boolean" => 34, //不正な論理値",
    "invalid_email" => 35, //不正なメールアドレス",
    "no_allowed_param" => 36, //パラメーターが許可されていない",
    "no_param" => 37, //パラメーターが何もセットされていない",
    "invalid_querystring" => 38, //不正なクエリー文字列",
    "missing_param" => 39, //必要なパラメーターがセットされていない",
    "invalid_param_key" => 40, //指定できない不正なパラメーターがある",
    "no_payment_method" => 41, //支払い手段がセットされていない",
    "payment_method_duplicate" => 42, //支払い手段が重複してセットされている",
    "payment_method_duplicate_including_customer" => 43, //支払い手段が重複してセットされている(顧客IDを含む)",
    "failed_payment" => 44, //指定した支払いが失敗している",
    "invalid_refund_amount" => 45, //不正な返金額",
    "already_refunded" => 46, //すでに返金済み",
    "invalid_amount_to_not_captured" => 47, //確定されていない支払いに対して部分返金ができない",
    "refund_amount_gt_net" => 48, //返金額が元の支払い額より大きい",
    "capture_amount_gt_net" => 49, //支払い確定額が元の支払い額より大きい",
    "invalid_refund_reason" => 50, //返金理由 refund_reason の値が不正",
    "already_captured" => 51, //すでに支払いが確定済み",
    "cant_capture_refunded_charge" => 52, //返金済みの支払いに対して支払い確定はできない",
    "cant_reauth_refunded_charge" => 53, //返金済みの支払いに対して再認証はできない",
    "charge_expired" => 54, //認証が失効している支払い",
    "already_exist_id" => 55, //すでに存在しているID",
    "token_already_used" => 56, //すでに使用済みのトークン",
    "already_have_card" => 57, //指定した顧客がすでに保持しているカード",
    "dont_has_this_card" => 58, //顧客が指定したカードを保持していない",
    "doesnt_have_card" => 59, //定期課金の作成APIで指定した引数customerがデフォルトカードを持っていない",
    "already_have_the_same_card" => 60, //すでに同じカード番号、有効期限のカードを保持している",
    "invalid_interval" => 61, //不正な課金周期",
    "invalid_trial_days" => 62, //不正なトライアル日数",
    "invalid_billing_day" => 63, //不正な支払い実行日",
    "billing_day_for_non_monthly_plan" => 64, //支払い実行日は月次プランにしか指定できない",
    "exist_subscribers" => 65, //購入者が存在するプランは削除できない",
    "already_subscribed" => 66, //すでに定期課金済みの顧客",
    "already_canceled" => 67, //すでにキャンセル済みの定期課金",
    "already_paused" => 68, //すでに停止済みの定期課金",
    "subscription_worked" => 69, //すでに稼働している定期課金",
    "cannot_change_prorate_status" => 7, //日割り課金の設定はプラン変更時のみ可能",
    "too_many_metadata_keys" => 71, //metadataキーの登録上限20を超過している",
    "invalid_metadata_key" => 72, //不正なmetadataキー",
    "invalid_metadata_value" => 73, //不正なmetadataバリュー",
    "apple_pay_disabled_in_livemode" => 74, //本番モードのApple Pay利用が許可されていない",
    "invalid_apple_pay_token" => 75, //不正なApple Payトークン",
    "test_card_on_livemode" => 76, //本番モードのリクエストにテストカードが使用されている",
    "not_activated_account" => 77, //本番モードが許可されていないアカウント",
    "payjp_wrong" => 78, //PAY.JPのサーバー側でエラーが発生している",
    "pg_wrong" => 79, //決済代行会社のサーバー側でエラーが発生している",
    "not_found" => 80, //リクエスト先が存在しないことを示す",
    "not_allowed_method" => 81, //許可されていないHTTPメソッド",
    "over_capacity" => 82, //レートリミットに到達",
    "refund_limit_exceeded" => 83, //期限を過ぎた後の返金操作",
    "cannot_prorated_refund_of_subscription" => 84, //返金期限を過ぎた為、定期課金の日割り返金が行えない",
    "three_d_secure_incompleted" => 85, //3Dセキュアフローが完了していない状態で別の操作を行った",
    "three_d_secure_failed" => 86, //3Dセキュア認証に失敗した",
    "not_in_three_d_secure_flow" => 87, //3Dセキュア対象外の支払いか、3Dセキュアフローが時間切れになった",
    "unverified_token" => 88, //3Dセキュアが完了していないトークンで支払いが行われた",
);

const CODE_TO_MESSAGE = array(
    0 => "決済ができませんでした（コード: %d)。",
    1 => "カード情報が正しくありません。",
    2 => "有効期限（月）が正しくありません。",
    3 => "有効期限（年）が正しくありません。",
    4 => "有効期限が切れています。",
);


\Payjp\Payjp::setApiKey($PAYJP_API["secret_key"]);


/**
 * PAYJP JAVASCRIPT用パラメタ返す
 * 
 * @return array
 */
function getUIparams()
{
    global $PAYJP_UI_EUC;
    return $PAYJP_UI_EUC;
}

//	最終支払いセット
/**
 * セッションのデータ使用して支払処理
 * 成功：$_SESSION["payjp"]["charge_id"]に支払番号追加
 * 失敗：$_SESSION['PAYJP_ERROR']をエラーエラーメッセージにする
 * 
 */
function payjpCharge()
{
    global $PAYJP_API,
        $API_ERROR_CODES,
        $ERROR_PREFIX,
        $ERROR_SUFFIX;

    $errors = array();
    $finalPayment = array(
        'card' => $_SESSION["payjp"]["card_token"],
        'amount' => $_SESSION["payjp"]["payment_amount"],
        'currency' => $PAYJP_API["currency"],
        'capture' => $PAYJP_API["capture"],
        'expiry_days' => $PAYJP_API["expiry_days"],
    );
    try {
        $charge = \Payjp\Charge::create($finalPayment);
    } catch (\Payjp\Error\Card $e) {
        $code_error = $API_ERROR_CODES[$e->getCode()];
        if (array_key_exists($code_error, CODE_TO_MESSAGE)) {
            $errors[] = CODE_TO_MESSAGE[$code_error];
        } else {
            $errors[] = sprintf(CODE_TO_MESSAGE[0], $code_error);
        }
    } catch (\Payjp\Error\InvalidRequest $e) {
        // Invalid parameters were supplied to Payjp's API
        $errors[] = API_INVALID_REQUEST_ERROR;
    } catch (\Payjp\Error\Authentication $e) {
        // Authentication with Payjp's API failed
        $errors[] = API_AUTHENTICATION_ERROR;
    } catch (\Payjp\Error\ApiConnection $e) {
        // Network communication with Payjp failed
        $errors[] = API_CONNECTION_ERROR;
    } catch (\Payjp\Error\Base $e) {
        // Display a very generic error to the user, and maybe send
        // yourself an email
        $errors[] = API_BASE_ERROR;
    } catch (Exception $e) {
        // Something else happened, completely unrelated to Payjp
        $errors[] = ERROR;
    }
    if (count($errors) > 0) {
        foreach ($errors as $err) {
            $ERROR[] = $err;
        }

        $_SESSION['PAYJP_ERROR'] = $ERROR;
        $url = HTTPS . CANCEL_PATH;
        header("Location: $url\n\n");
        exit;
    }
    $_SESSION["payjp"]["charge_id"] = $charge["id"];
}

/**
 * 支払い情報のメモを更新
 * 
 * @param string $id PAYJP支払い番号
 * @param string $description メモテキスト
 * 
 * @return Void
 */
function updatePayjpChargeDescription($id, $description)
{
    $ch = \Payjp\Charge::retrieve($id);
    $ch->description = $description;
    $ch->save();
}

/**
 * 支払い情報のMetadataを更新
 * @link https://pay.jp/docs/api/#metadata PAYJP API Metadata Documentation
 * @param string $id PAYJP支払い番号
 * @param array $metadata メモテキスト
 * 
 * @return Void
 */
function updatePayjpChargeMetadata($id, $metadata)
{
    if (!is_array($metadata) || count($metadata) > 20) return;
    $ch = \Payjp\Charge::retrieve($id);
    $ch->metadata = $metadata;
    $ch->save();
}
