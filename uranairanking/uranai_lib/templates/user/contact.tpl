{extends file="main.tpl"}
{block name=seo}
  <title>運営会社 12星座占いランキング</title>
  <meta name="keywords" content="12星座占いランキング,占い,ランキング,運営会社">
  <meta name="description" content="12星座占いランキングのお問い合わせ">

  <!--OGP START-->
  {include file="ogp.tpl" title="|お問い合わせ" des="12星座占いランキングのお問い合わせ"}
  <!--OGP END-->
{/block}

{block name=body}
  <div class="container company">
    <div class="title row text-center">
      <h2 class="font-color">お問い合わせ</h2>
      {include "mainline.parts.tpl"}
    </div>

    <div class="col-md-9 spadding-top">
      <div class="tecen base-bg contents-space">
        <!--ここからエラー表示-->
        <p>
          <!--ERROR-->
        </p>
        <p>
          <!--ASK-->
        </p>
        <!--ここまでエラー表示-->
        <form action="{sitelink mode="contact/send"}" method="POST">
          <fieldset>
            <span>下記のフォームにご入力いただき、内容をご確認の上「送信」ボタンを押してください。<br><em class="detail_default_graph_red">*</em>は必須項目です。</span>
            <dl class="contact-container">
              <li class="contact-li">
                <div class="contact-div">
                  <label class="contact-list" for="name">お名前<em class="detail_default_graph_red">*</em></label>
                  <div class="detail_default_graph_red">
                    {foreach $errors.name as $data}
                      {$data}
                    {/foreach}
                  </div>
                </div>
                <div class="contact-input">
                  <input type="text" size="12" name="name" value="{$smarty.post.name}" class="contact-text">
                </div>
              </li>
              <li class="contact-li">
                <div class="contact-div">
                  <label class="contact-list" for="furigana">フリガナ<em class="detail_default_graph_red">*</em></label>
                  <div class="detail_default_graph_red">
                    {foreach $errors.furigana as $data}
                      {$data}
                    {/foreach}
                  </div>
                </div>
                <div class="contact-input">
                  <input type="text" name="furigana" value="{$smarty.post.furigana}">
                </div>
                

              </li>
              <li class="contact-li">
              <div class="contact-div">
                <label class="contact-list" for="tel">ご連絡先電話番号</label>
                <div class="detail_default_graph_red">
                  {foreach $errors.tel as $data}
                    {$data}
                  {/foreach}
                </div>
              </div>
              <div class="contact-input">
                <input type="tel" size="12" name="tel" value="{$smarty.post.tel}">
              </div>
              </li>
              <li class="contact-li">
                <div class="contact-div">
                  <label class="contact-list" for="email">メールアドレス<em class="detail_default_graph_red">*</em></label>
                  <div class="detail_default_graph_red">
                    {foreach $errors.email as $data}
                      {$data}
                    {/foreach}
                  </div>
                </div>
                <div class="contact-input">
                <input type="email" name="email" value="{$smarty.post.email}">
              </div>
              </li>

              <li class="contact-li">
                <div class="contact-div">
                  <label class="contact-list" for="confirm_email">メールアドレス【確認用】<em class="detail_default_graph_red">*</em></label>
                  <div class="detail_default_graph_red">
                    {foreach $errors.confirm_email as $data}
                      {$data}
                    {/foreach}
                  </div>
                </div>
                <div class="contact-input">
                  <input type="email" name="confirm_email" value="{$smarty.post.confirm_email}">
                </div>

              </li>
              <li class="contact-li">
                <div class="contact-div">
                  <label class="contact-list" for="comments">お問い合わせ内容<em class="detail_default_graph_red">*</em></label>

                  <div class="detail_default_graph_red">
                    {foreach $errors.comments as $data}
                      {$data}
                    {/foreach}
                  </div>
                </div>
                {* <textarea class="contact-textarea" type="text" name="comments" value="{$smarty.post.comments}"></textarea> *}
                <textarea class="contact-textarea" type="text" name="comments">{$smarty.post.comments}</textarea>

                {* コメント<textarea name="eve_sch_date" value="<?php print($_SESSION["comment"]); ?>" cols="50" rows="5"></textarea><br/> *}
              {* コメント<textarea name="eve_sch_date" cols="50" rows="5"><?php print($_SESSION["comment"]); ?></textarea><br/> *}


              </li>
            </dl>
          </fieldset>
          <p id="mailformbtnarea"><input type="button" onclick="submit();" value="送信"></p>
                  <input type="hidden" name="contact_token" value="{$contactToken}">
        </form>
        <p style="clear: both; text-align:center; word-break: keep-all;">※お問い合せ内容によっては、お時間をいただく場合がございますので、あらかじめご了承ください。
        </p>
      </div>
    </div>
    {include file='sidebar.tpl'}
  </div>
{/block}