<?php
/**
 * URL階層のクラス化
 *
 * @property string  $dir      URL階層(https://uranairanking.jp/foo/bar/ ... の /foo/単位)
 * @property string  $lastmod  最終更新日
 * @property string  $priority 優先度
 * @property array   $chidren  子ノードクラスをいれる用の配列
 * @property object  $parent   親ノードクラス
 * @property boolean $quiet    この階層を出力するか(デフォルト OFF)
 *
 */
class UrlNode
{
    protected $dir;
    protected $lastmod;
    protected $priority;

    public $children = array();
    public $parent;
		public $quiet  = false;

		/**
		 * コンストラクタ
		 * @param string $dir 作成するURL階層の名前
		 * @param object $parent 親オブジェクト
		 *
		 */
    public function __construct($dir, $parent)
    {
        $this->dir = $dir;
        $this->parent = $parent;
    }

		/**
		 * 子ノードを追加する
		 * @param object $child 子ノードオブジェクト
		 */
    public function addChild($child)
    {
        $this->children[] = $child;
    }

		/**
		 * 自身から親までのURL階層をすべて連結して返す
		 * 自分から親までたどります
		 * (https://ひい爺さん/爺さん/父さん/自分)
		 * @return string くっつけたURL
		 */
    public function parentPath()
    {
        $trail = "/";

        if ($this->dir == "") {
            $trail = "";
        }
        if ($this->parent) {
            return $this->parent->parentPath(). $this->dir . $trail;
        } else {
            return $this->dir;
        }
    }

		/**
		 * 優先度をセット
		 * @param string $priority
		 */
		public function setPriority($priority){
			$this->priority = $priority;
		}

		/**
		 * 最終更新日をセット
		 * @param string $lastmod
		 */
		public function setLastMod($lastmod){
			$this->lastmod = $lastmod;
		}

		/**
		 * 自身を<url>...</url>の形に整形して返す
		 * 最終更新日はセットされていなければ省略可)
		 */
    public function lines()
    {
				$result = "";

				if(!$this->quiet){
					$result .= "\t<url>\n";
					$result .= "\t\t<loc>".$this->parentPath()."</loc>\n";
					$result .= "\t\t<priority>".$this->priority."</priority>\n";
					if($this->lastmod != ""){ $result .= "\t\t<lastmod>".$this->lastmod."</lastmod>\n"; }
					$result .= "\t</url>\n";
				}

        foreach ($this->children as $child) {
        	$result .= $child->lines();
        }
        return $result;
    }

		/**
		 * 自身の出力をOFFにする
		 *
		 */
    public function quiet()
    {
			$this->quiet = true;
    }
}

/**
 * サイトマップ.xmlファイルのクラス化
 *
 * @const    string  HEAD      xmlファイルの先頭
 * @const    string  BOTTOM    xmlファイルの末尾
 * @property object  $node     UrlNodeオブジェクト(の一番の祖先になってるもの一つ)
 */
class Xml
{
    const HEAD = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">\n";
    const BOTTOM = "</urlset>";
    private $node;

		/**
		 * コンストラクタ
		 * @param object $node UrlNodeオブジェクト
		 */
    public function __construct($node)
    {
        $this->node = $node;
    }

		/**
		 * ノードをxmlファイルの形に整形
		 * @return string $result サイトマップ用文字列
		 */
    public function make()
    {
        $result = "";
        $result .= self::HEAD;
        $result .= $this->node->lines();
        $result .= self::BOTTOM."\n";
        return $result;
    }

}
