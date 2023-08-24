<?php

// モーダル画像パスの読み込み
require_once('modal_element_generat.php');


/**
 *期間限定のイベントクラス
 *
 * 各イベントの設定を行っている。
 *
 */
class SeasonEvent
{
	protected $folder;
	protected $modal_img = [];
	protected $messages;
	protected $event_key;
	protected $modal = [];

	/**
	 * __construct
	 *
	 * イベントを識別する文字列
	 * @param  mixed $event_key
	 *
	 * イベント画像フォルダパス
	 * @param  mixed $folder
	 *
	 * 各星座毎のメッセージ
	 * @param  mixed $messages
	 *
	 * @return void
	 */
	public function __construct(string $event_key, string $folder, array $messages)
	{
		$this->event_key = $event_key;
		$this->folder = $folder;
		$this->messages = $messages;
	}


	/**
	 * getBannerPath
	 * イベントバナー画像の取得
	 *
	 * @return void
	 */
	public function getBannerPath()
	{
		return "/user/event_img/{$this->folder}/banner_";
	}


	/**
	 * getIconPath
	 * 星座番号に対応したイベントアイコンの取得
	 *
	 * 各星座番号
	 * @param  mixed $sign_
	 * @return void
	 */
	public function getIconPath(int $sign_)
	{
		return "/user/event_img/{$this->folder}/{$sign_}.png";
	}


	/**
	 * getModalImagePath
	 * モーダルイメージ画像パスを返却する。
	 *
	 * @return array
	 */
	public function getModalImagePath()
	{
		for ($i = 0; $i < 12; $i++) {
			$this->modal_img[$i] = "/user/event_img/" . $this->folder . "/". ($i + 1) ."-modal.png";
		}
		return $this->modal_img;
	}


	/**
	 * getMessage
	 * 配列キー（星座番号）に応じたメッセージを返却する。
	 *
	 * 各星座番号
	 * @param  mixed $sign_
	 * @return void
	 */
	public function getMessage(int $sign_)
	{
		return $this->messages[$sign_];
	}



    /**
     * getEventData
     * イベント用バナー画像パス、イベントキーを返却する。
	 *
     * @return array
     */
    public function getEventData() :array{

		return [
			'banner_path' => $this->getBannerPath(),
			'event_key' => $this->event_key
		];
	}



	/**
	 * getEventData
	 * 各星座番号に対応したメッセージとアイコン、またイベントキーを返却する。
	 *
	 * 各星座番号
	 * @param  mixed $sign_
	 * @return array
	 */
	public function getEventSignData(int $sign_) :array{

		return [
			'icon_path' => $this->getIconPath($sign_),
			'message' => $this->getMessage($sign_),
			'event_key' => $this->event_key
		];
	}


	/**
	 * ModalGeneration
	 * イベントキーに対応した画像パスをセットしたHTMLタグ(li, a)を返却する。
	 *
	 * @return array
	 */
	public function ModalGeneration() {

		$modal = new ModalElement;
		$event_key = $this->event_key;
		$image_path = $this->getModalImagePath();

		$this->modal = $modal->MakeElement($event_key, $image_path);

		return $this->modal;

	}

}


// 猫の日イベント add 2023/02/20
/**
 * 各イベントクラス
 *イベント用画像フォルダ名と、イベントメッセージ、イベントキーを配列として設定する。
 *index.phpでインスタンス化して呼び出す。
 */
class CatEvent extends SeasonEvent
{

	/**
	 * __construct
	 *
	 * @return void
	 */
	public function __construct()
	{
		parent::__construct(
			"cat",
			'cat_20230222',
			[
				// みずがめ座
				1 => "<p>社会性高いけれど、自由を愛するみずがめ座さん。</p>
						<p>飼い主さんが他の猫と楽しげにしているだけで嫉妬をしちゃうこともあるほど、飼い主大好きで、忠実な<b>「ロシアンブルー」</b>みたいです。</p>
						<p>はじめは人見知りをしますが、飼い主と認めた相手には献身的な愛情を持つため「犬のような猫」と例えられることもしばしば。<br>
						また賢く、おだやかな性格で鳴き声をあげないことが多く、「ボイスレスキャット」という呼び名もあります。<br>
						ただし、性格はストレスを溜め込みやすいところも。<br>
						環境の変化を始め、他のペットをかまったり、留守が多くなりがちだったりするとストレスで凶暴化する場合があります。<br>
						だからこそ、飼い主は考えて気持ちを汲み取ってあげることが大切です。</p>",

				// うお座
				2 => "<p>慈愛にあふれ共感力が高い一方、傷つきやすいところもあるうお座さん。</p>
						<p>あなたは<b>「シンガプーラ」</b>のようです。とてもおとなしく、細やかな愛情を持っています。</p>
						</p>シンガプーラは、飼い主さんや家族などへの愛情が細やかでやさしく、人の話すことが理解できるといわれるくらい、とても賢い猫です。<br>
						性格は優しく甘えん坊で鳴き声も小さく、とてもおとなしいのが特徴。<br>
						ただ、神経が過敏な面もあり、ほかの猫と一緒に飼うと独占欲で喧嘩をすることも。</p>",

				// おひつじ座
				3 => "<p>エネルギッシュで自立心が強く、自分の意見をはっきりと述べるおひつじ座さん。</p>
						<p>そんなあなたを猫に例えるのなら、身体能力が高く体力に優れ、遊ぶのも大好きな、<b>「アメリカンショートヘア」</b>！</p>
						<p>アメリカンショートヘアは筋肉質で頑丈な体を持つので運動が得意。<br>
						人なつこいファニーフェイスが何とも愛らしく、人や他の猫ともすぐ仲よくなれる、フレンドリーさが魅力です。</p>",

				// おうし座
				4 => "<p>優しくて穏やか、忍耐強いおうし座さん。</p>
						<p>あなたは<b>「ノルウェージャン・フォレスト・キャット」</b>に似ているかも。</p>
						<p>元々は寒冷な北欧地域で飼われており、外見が野生的で美しく、大柄な体格を持つ猫種の1つです。<br>
						性格は穏やかで落ち着いており、家族との触れ合いを楽しむ猫種として知られています。<br>
						人間の子供にいたずらされても、怒ることなくじっと耐える我慢強さもあります。</p>",

				// ふたご座
				5 => "<p>好奇心旺盛でコミュニケーション能力が高いふたご座さん。</p>
						<p>そんなあなたは猫なら<b>「マンチカン」</b>。活発で社交的な性格で、非常に好奇心が強いのが特徴です。</p>
						<p>短い足が有名ですが、以外と筋肉質で運動能力が高く、ジャンプ力は他の猫と変わりません。<br>
						元気な性格でイタズラ好きな面もあります。</p>",

				// かに座
				6 => "<p>保守的で母性が強く、孤独が苦手なかに座さん。</p>
						<p>猫にたとえるのなら、家族に対して愛情深く接し、穏やかな<b>「ラグドール」</b>です。</p>
						<p>ラグドールの名前の由来は英語で「（布製の）ぬいぐるみ」を意味する「ragdoll」（ラグドール）からで、<br>
						「抱き上げるとまるでぬいぐるみのようにおとなしくしている」ところから来ているそう。<br>
						名前の通り抱っこも大好きな、人懐っこい猫です。</p>",

				// しし座
				7 => "<p>誇り高く、自信にあふれるしし座さん。</p>
						<p>性格的に、甘えん坊だけど、ちょっと自己主張の激しいところのある<b>「シャム猫」</b>に似ています。</p>
						<p>シャム猫はエレガントで洗練された外見とは裏腹に、活発で社交的な性格をしています。<br>
						人と触れ合うことが大好きで、寒がりな傾向があるのでひざや肩などに乗って、つねに飼い主さんの体温を感じていたい猫です。<br>
						またシャム猫は、よく鳴く猫としても知られています。飼い主さんに向かって大きな声で話しかけるようにアピールすることが多くあるようです。</p>",

				// おとめ座
				8 => "<p>穏やかで控えめ、気遣いのできるおとめ座さん。</p>
						<p>そんなあなたは<b>「メインクーン」</b>のような人かも。</p>
						<p>メインクーンはの性格は温厚で穏やか、とてもやさしいのが特徴です。<br>
						ネズミを退治する「ワーキングキャット」として人と古くから共生してきた歴史から、穏やかで人懐っこい性格が多いとされています。<br>
						ただし、必要以上にベタベタされるのは少し苦手なようなので、程よい距離感で接してあげると良いかもしれません。</p>",

				// てんびん座
				9 => "<p>社交的でほがらか、バランス感覚に優れたてんびん座さん。</p>
						<p>甘えん坊で人懐っこく、おっとり系の<b>「スコティッシュフォールド」</b>のようです。</p>
						<p>垂れた耳が特徴のやさしい外見同様にとても穏やかな性格で、おとなしく、あまり激しく動き回る性格ではないと言われています。<br>
						また、飼い主さんに対しては、すぐすり寄ってくる、甘えん坊な一面も見られます。<br>
						人懐っこく愛情深いので、赤ちゃんや猫、他の動物とも良好な関係を築けることができ、協調性や順応性が高いといわれています。</p>",

				// さそり座
				10 => "<p>喜怒哀楽が激しく、興味のある分野を追い求めるさそり座さん。</p>
						<p>その性格は、知性が高く、イタズラや遊び好きで、甘えん坊な一面もあるけど、基本ベタベタするのをあまり好まない<spna>「ベンガル」</span>に似ています。</p>
						<p>ベンガルの性格は飼い猫用に向くように改良されてきたので、野性的な面を残しつつも温和でフレンドリーです。<br>
						甘えん坊な側面もありますが、人から必要以上にベタベタされることはあまり好まない傾向が。<br>
						よく鳴くおしゃべり好きなタイプが多いです。<br>
						そして、野性の血が騒ぐのか、大の遊び好き。また、水が苦手でないことも猫としては珍しいといわれています。</p>",

				// いて座
				11 => "<p>ハンター気質で好奇心が強い、いて座さん。</p>
						<p>そんなあなたはを猫に例えると、気になるおもちゃに一直線で好奇心おう盛で遊び好きな<b>「ラガマフィン」</b>です。</p>
						<p>ラガマフィンは人なつっこく、飼い主さんと触れ合うことも大好き。<br>
						甘えん坊な一方、好奇心おう盛で遊び好きな一面も。<br>
						おもちゃを目にしたらすぐに興味を示すような機敏さも兼ね備えています。<br>
						非常に愛情深く、子供とも仲良くできるやさしさも持っています。</p>",

				// やぎ座
				12 => "<p>理想の高い努力家のやぎ座さん。</p>
						<p>自立心が高く、他の猫に比べお留守番も苦にならない性格だといわれている<b>「ブリティッシュショートヘアー」</b>のような人です。</p>
						<p>おおらかで、落ち着きがある性格のブリティッシュショートヘアー。<br>
						もともと狩猟能力に優れ、農家などで飼われていたこともあり、賢いことでよく知られています。<br>
						運動能力も高いので、テレビやハリウッド映画などにもよく登場する猫種のひとつです。<br>
						自己アピールするときは、犬のように尾をゆったりと左右に振る独特の仕草をします。</p>"
			],
		);
	}
}



// 2023/03/29 春の新入生・新社会人イベント
/**
 * SpringEvent
 *
 * イベントキーは「spring」
 */
class SpringEvent extends SeasonEvent
{
	public function __construct()
	{
		parent::__construct(
			"spring",
			'spring_20230329',
			[
				// みずがめ座
				1 => "みずがめ座",
				// うお座
				2 => "うお座",
				// おひつじ座
				3 => "開運小物：赤いアイテム 牡羊座は行動的な星座で、活力にあふれています。<br>
				そのため、赤いアイテムが運気を高め、勇気や情熱を与えてくれます。",
				// おうし座
				4 => " 開運小物：緑のアイテム 牡牛座は忍耐強く、現実的な星座です。<br>
				そのため、緑のアイテムが運気を高め、安定感やバランス感覚を与えてくれます。",
				// ふたご座
				5 => "開運小物：コミュニケーショングッズ 双子座はコミュニケーション能力が高く、好奇心旺盛な星座です。<br>
				そのため、コミュニケーショングッズが運気を高め、人間関係やコミュニケーション能力を向上させてくれます。",
				// かに座
				6 => "開運小物：お守りやアクセサリー 蟹座は家族や友人との絆を大切にする星座で、感受性が豊かです。<br>
				そのため、お守りやアクセサリーが運気を高め、感性や直感力を磨いてくれます。",
				// しし座
				7 => " 開運小物：金のアイテム 獅子座は自信にあふれ、リーダーシップがあります。<br>
				そのため、金のアイテムが運気を高め、自信や財運を呼び込んでくれます。",
				// おとめ座
				8 => " 開運小物：整理整頓グッズ 乙女座は細かいところまで気が回り、整理整頓が得意な星座です。<br>
				そのため、整理整頓グッズが運気を高め、集中力や創造力を高めてくれます。",
				// てんびん座
				9 => " 開運小物：美術品や装飾品 天秤座は美的センスが高く、バランス感覚に優れています。<br>
				そのため、美術品や装飾品が運気を高め、美的センスやバランス感覚を向上させてくれます。",
				// さそり座
				10 => "開運小物：水晶やパワーストーン 蠍座は直感力が鋭く、スピリチュアルなことに興味があります。<br>
				そのため、水晶やパワーストーン",
				// いて座
				11 => "いて座",
				// やぎ座
				12 => "やぎ座"

			]
		);
	}
}