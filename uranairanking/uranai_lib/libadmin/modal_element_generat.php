<?php


class ModalElement
{

    protected $zodiac_sign = [];
    protected $html;


    /**
     * SignDefinition
     * 星座の配列生成
     *
     * @return array
     */
    public function SignDefinition()
    {
        $this->zodiac_sign = [
            "aeris",
            "taurus",
            "gemini",
            "cancer",
            "leo",
            "virgo",
            "libra",
            "scorpio",
            "sagittarius",
            "capricorn",
            "aquarius",
            "pisces"
        ];

        return $this->zodiac_sign;
    }


    /**
     * MakeElement
     * モーダル画像生成
     *
     * イベントキー
     * @param  mixed $event_key
     *
     * モーダル画像パス
     * @param  mixed $modal_img_path
     *
     * @return string
     */
    function MakeElement(string $event_key, array $modal_icon_path) {

        $signs = $this->SignDefinition();

        foreach($signs as $key => $value) {

            $this->html .= "<li><a href=\"/".$value."/\">\n";
            $this->html .= "<img src=\"$modal_icon_path[$key]\" class=\"season-img send-season-event\" alt=\"$event_key.$value\">\n";
            $this->html .= "</a></li>\n";

        }

        return $this->html;

    }
}
