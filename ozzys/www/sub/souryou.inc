<?PHP
//	送料関係設定ファイル

//	送料
$UN_L = array("","940","740","740","640","740","640","640","640","640","640",
	"640","640","640","640","640","640","640","640","640","640",
	"640","640","640","640","640","640","640","640","640","640",
	"740","740","740","740","740","740","740","740","740","940",
	"940","940","940","940","940","940","1060");

//	送料用都道府県並び
//	("","北海道","青森県","岩手県","宮城県","秋田県","山形県","福島県","茨城県","栃木県","群馬県",
//	"埼玉県","千葉県","東京都","神奈川県","新潟県","富山県","石川県","福井県","山梨県","長野県",
//	"岐阜県","静岡県","愛知県","三重県","滋賀県","京都府","大阪府","兵庫県","奈良県","和歌山県",
//	"鳥取県","島根県","岡山県","広島県","山口県","徳島県","香川県","愛媛県","高知県","福岡県",
//	"佐賀県","長崎県","熊本県","大分県","宮崎県","鹿児島県","沖縄県");


//	追加送料設定中分類コード (ロッド用) (305=中古は含まれていません)
$T_SOU_L = array ('105','205','405','805');


//	追加送料 (simon : 2018-07-06から未使用)
$tui_sou = 1000;

// 新しい竿送料仕組み使う時に、以下のフラグをtrueにしてください。
// falseの場合は、以上にある $tui_sou が利用されています。
define('ROD_FEE_METHOD_NEW', true);

//	代引き手数料
$daibiki = 300;


//	送料無料機能利用
//	add ookawara 2016/07/20
//	0:利用しない	1:利用する
define('SOURYOUFREE', 1);
// define('SOURYOUFREE', 0); // TEST

//	送料無料金額
//	0より大きい場合、その金額が送料無料となる。消費税も含めての金額を設定して下さい。
define('SOURYOUFREEPRICE', 0);
// define('SOURYOUFREEPRICE', 2000); // TEST
// define('SOURYOUFREEPRICE', 21000); // TEST
// define('SOURYOUFREEPRICE', 41000); // TEST

define('ADD_ROD_SHIPPING_FEE_ANYWAY', true);
// trueの場合は、送料が無料でも、竿のフラグに対して、送料追加する
// falseの場合は、竿の別送料を追加しない

class Souryou {
    // add simon 2018-07-03 >>>
    static $ROD_SOURYOU_L = array(
        'X' => 1000,
        'Y' => 2000,
        'Z' => 3000,
    );
    // <<<


    /**
       送料無料機能がON・OFFの確認
       @return boolean
     */
    static function can_be_free() {
        return SOURYOUFREE == 1;
    }


    /**
       カートのアイテムの中に、送料無料アイテムがあるか確認
       @param array $cart_
       アイテムに必要のデータ:
         - goods.set_flag
       @return boolean
     */
    static function cart_has_free_shipping_items($cart_) {
        $free_postage_flag = 0;

        foreach($cart_ as $item) {
            // print_r($item);
            if (self::can_be_free() && $item['free_postage'] > 0) {
                $free_postage_flag = true;	//送料無料	//	金額での送料無料が出てない
                break;
            }
        }

        return $free_postage_flag;
    }


    /**
       カートのアイテムの中に、別送料の竿があるか確認
       @param array $cart_
       アイテムに必要のデータ:
         - goods.set_flag
       @return boolean
     */
    static function cart_has_rods_with_fee($cart_) {
        $has_rods = false;

        foreach($cart_ as $item) {
            // print_r($item);
            if(self::prod_has_rod_fee($item)) {
                $has_rods = true;
                break;
            }
        }

        return $has_rods;
    }


    /**
       商品データで、竿送料フラグがあるか、確認するメソッド
       @param array $item_ (goods.set_flag データが必要)
       @return boolean
     */
    static function prod_has_rod_fee($item_) {
        return isset($item_['set_flag']) && isset(self::$ROD_SOURYOU_L[$item_['set_flag']]);
    }


    /**
       商品データで、竿送料を出す（可能であれば）
       @param array $item_ (goods.set_flag データが必要)
       @return int / null (set_flagがない場合)
     */
    static function prod_rod_fee($item_) {

        $price = null;
        if(self::prod_has_rod_fee($item_)) {
            $price = self::$ROD_SOURYOU_L[$item_['set_flag']];
        }

        return $price;
    }


    /**
       カートのアイテムの中に、一番高い竿の送料を出すメソッド
       @param array $cart_
       アイテムに必要のデータ:
         - goods.set_flag
       @return int
     */
    static function cart_rod_shipping_fee($cart_) {
        $rod_fee = 0;

        foreach($cart_ as $item) {
//            print_r($item);

            if(ROD_FEE_METHOD_NEW) {
                // NEW price per rod method
                if(self::prod_has_rod_fee($item)) {
                    $rod_fee = max($rod_fee, self::$ROD_SOURYOU_L[$item['set_flag']]);
                }
            }
            else {
                // OLD METHOD
                global $T_SOU_L, $tui_sou;
                $un = false;
                foreach ($T_SOU_L AS $val) {
                    if ($val == $item['class_m']) {
                        $un = true;
                        break;
                    }
                }
                if($un) {
                    $rod_fee = $tui_sou;
                }
            }
        }

        return $rod_fee;
    }


    /**
       どのくらい商品の追加で無料送料になりますか、計算するメソッド
       @return int (positive for what need to be added to get free shipping) or null if SOURYOUFREEPRICE is 0 (inactive value)
    */
    static function cart_amount_until_free_shipping($cart_total_) {
        if(SOURYOUFREEPRICE > 0) {
            return SOURYOUFREEPRICE - $cart_total_;
        }
        else {
            return null;
        }
    }


    /**
       カートデータで送料を計算する
       @param array $cart_ (アイテム一覧)
       @param int $fee_detault_ (エリアに対して基本送料)
       @param int $total_ (カートのアイテムの合計)
       @return int カートデータに対しての送料
    */
    static function calculate($cart_, $fee_default_, $total_) {
        // default fee (user delivary address)
        $fee = $fee_default_;
        // print $fee;
        $is_free = false;

        if(self::can_be_free()) {
            // free shipping option ON
            $rest = self::cart_amount_until_free_shipping($total_);
            if($rest!==null && $rest <= 0) {
                $fee = 0;
                $is_free = true;
            }
            elseif(self::cart_has_free_shipping_items($cart_)) {
                $fee = 0;
                $is_free = true;
            }
        }

        if(!$is_free || ADD_ROD_SHIPPING_FEE_ANYWAY) { // edit simon 2020-04-23
            // no free shipping
            // print_r($cart_);
            // add rods?
            $fee += self::cart_rod_shipping_fee($cart_);
        }

        return $fee;
    }


    /**
     * カート詳細出すメソッド
     * とりあえず、class_mとset_flgを出す
     * @global connection $db
     * @return array
     */
    static function fetchCartDetails() {
        global $db;

        $items = array();

        foreach($_SESSION['customer'] as $list_num => $qty) {
            $sql = "SELECT l.list_num, l.pluid, l.class_m, g.set_flag"
                . " FROM list l"
                . " JOIN goods g ON g.pluid=l.pluid"
                . " WHERE l.list_num='$list_num' LIMIT 1";
            //print $sql;

            if ($result = pg_query($db, $sql)) {
                $item = pg_fetch_assoc($result);
                $items[] = $item;
            }
        }

        return $items;
    }


    /**
     * カート内容によって、代引きが可能かどうか
     *
     * @param array$cart_
     * @return boolean
     */
    static function canDaibiki($cart_) {
        // default
        $ok = true;

        foreach($cart_ as $item) {
            if($item['set_flag']==="Y" || $item['set_flag']==="Z") {
                $ok = false;
                break;
            }
        }

        return $ok;
    }


    /**
     * カート内容によって、会社を選ぶ
     *
     * @param array$cart_
     * @return string
     */
    static function getDeliveryCompany($cart_) {
        // default
        $company = "ヤマト運輸";

        foreach($cart_ as $item) {
            if($item['set_flag']==="Y" || $item['set_flag']==="Z") {
                $company = "佐川急便";
                break;
            }
        }

        return $company;
    }
}
?>
