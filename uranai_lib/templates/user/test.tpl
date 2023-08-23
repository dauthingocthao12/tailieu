<!--テスト用のテンプレートです。必要時以外本番環境にUPしないでください。-->

{extends file="main.tpl"}
{block name=body}

<style>
    .testing li {
        background: hotpink;
        display: inline-block;
        margin: 2px;
        padding: 5px;
        font-size: larger;
    }
</style>

<ul class="testing">
    <li>0.2<br>
        {insert siteEvaluationStars evaluation=0.2}
    </li>
    <li>0.6<br>
        {insert siteEvaluationStars evaluation=0.6}
    </li>
    <li>1.499<br>
        {insert siteEvaluationStars evaluation=1.499}
    </li>
    <li>3.1<br>
        {insert siteEvaluationStars evaluation=3.1}
    </li>
    <li>4<br>
        {insert siteEvaluationStars evaluation=4}
    </li>
    <li>4.5<br>
        {insert siteEvaluationStars evaluation=4.5}
    </li>
    <li>4.51<br>
        {insert siteEvaluationStars evaluation=4.51}
    </li>
    <li>4.999<br>
        {insert siteEvaluationStars evaluation=4.999}
    </li>
    <li>5<br>
        {insert siteEvaluationStars evaluation=5}
    </li>
</ul>

<style>
ul {
    list-style: none;
    padding: 0;
}
.ad-tests li {
    background: white;
    margin: 1em 0;
}
.ad-tests a {
    display: inline-block;
    padding: 1em;
}
</style>

<h2>iOS appli ad links tests</h2>

<ul class="ad-tests">
    <li><a href="http://www.azet.jp">http://www.azet.jp</a></li>
    <li><a href="https://www.azet.jp">https://www.azet.jp</a></li>

    <li><a href="http://www.azet.jp" target="_blank">http://www.azet.jp (_blank)</a></li>
    <li><a href="https://www.azet.jp" target="_blank">https://www.azet.jp (_blank)</a></li>

    <li><a href="http://www.azet.jp" target="_top">http://www.azet.jp (_top)</a></li>
    <li><a href="https://www.azet.jp" target="_top">https://www.azet.jp (_top)</a></li>

    <li><a href="mailto:simon@azet.jp">mailto:simon@azet.jp</a></li>
    <li><a href="mailto:simon@azet.jp" target="_blank">mailto:simon@azet.jp (_blank)</a></li>

    <li><a href="itmss://itunes.apple.com/jp/app/line%E5%8D%A0%E3%81%84/id595265709?mt=8">LINE占い - LINE Corporation (APP itmss)</a></li>
    <li><a href="itms-appss://itunes.apple.com/app/id595265709?mt=8" target="_blank">LINE占い - LINE Corporation (APP itms-appss)</a></li>
</ul>

{/block}