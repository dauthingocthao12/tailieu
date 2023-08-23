{extends file="main.tpl"}
{block name=seo}
<title>ユーザー情報 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,ユーザー情報">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。ユーザー情報の登録、変更ページです。">
{/block}
{block name=body}

<link rel="stylesheet" href="/formCheck/css/formCheck.css">
<script src="/formCheck/js/formCheck.js"></script>
<script src="/user/js/account.js"></script>

<div class="account container">
	<div class="page-center">
		<div class="col-sm-6 col-sm-offset-3">

			<p>
				{if $user}
				<a href="{sitelink mode="mypage"}" class="btn btn-primary"><i class="fa fa-arrow-left"></i> マイページへ戻る</a>
				<a href="{sitelink mode="account/logout"}" class="btn btn-danger"><i class="fa fa-unlock"></i> ログアウト</a>
				{/if}
			</p>

			{if $registration_db_error}
			<div class="alert alert-danger">
				<b> エラー： </b><br>
				このメールアドレスは既に登録されています。
			</div>
			{/if}

			{if $sql_error}
			<div class="alert alert-danger">
				<b> エラー： {$sql_error}</b><br>
				処理できませんでした。
			</div>
			{/if}

			<!--new account-->
			
			{if $user.user_id}
			<h2 class="font-color tecen"><i class="fa fa-user"></i> ユーザー情報</h2>
			{else}
			<div class="alert alert-info">
				<i class="fa fa-info-circle" aria-hidden="true"></i> まだユーザー登録は完了していません</div>
				<h2 class="font-color tecen"><i class="fa fa-user-plus"></i> 新規登録</h2>
				{/if}

				<div class="base-bg contents-space">

					{if $msg1st}
					<div class="alert alert-warning">{$msg1st}</div>
					{/if}

					<form action="" method="post">

						<div class="tecen account-img">
							{insert userAvatar avatar=$user.avatar}
							<a href="#change-avatar" class="btn btn-change-avatar"><i class="fa fa-fw fa-pencil-square"></i></a>
						</div>
						<div class="change-avatar-panel clearfix" style="display: none;">
							{foreach $avatars as $avatar}
							<label class="avatar">
								<input type="radio" name="avatar" value="{$avatar}" {if $avatar==$user.avatar || $user.avatar=='' && $avatar==$avatar_default}checked="checked"{/if}>
								{insert userAvatar avatar=$avatar}
							</label>
							{/foreach}
						</div>

						<label><span class="required">※ 必須</span></label>
						<div class="form-group">
							<label for="email">メールアドレス</label>
							<input class="form-control"
							id="email"
							name="email"
							type="text"
							{if $user}
							readonly 
							value="{$formData.email|default:$user.email}">
							<div class="alert alert-info">
								メールアドレスの変更は出来ません。<br>
								変更したい場合は、このユーザーを削除して、もう一度登録し直してください。<br>
								(このページ下部からユーザー削除へ進めます)<br/>
							</div>
							{else}
							readonly
							value="{$formData.email|default:$newUserMail}">
							{/if}
						</div>

						{if $pw_1st_required}
						<input type="hidden" name="pw_1st_required" value="1">
						{/if}

						<div class="form-group">
							<label for="password">パスワード{if $pw_1st_required}<span class="required"></span>{/if}</label>
							<input class="form-control" id="password" name="password" type="password">
							<div class="alert alert-info">
								パスワードは、8文字以上の英数字を指定して下さい。<br>
								なお、英小文字、英大文字、数字それぞれを１文字以上含めて下さい。<br>
								例：aBc12345
							</div>
						</div>
						<div class="form-group">
							<label for="password2">パスワード確認{if $pw_1st_required}<span class="required"></span>{/if}</label>
							<input class="form-control" id="password2" name="password2" type="password">
						</div>
						<div class="form-group">
							<label for="handlename">ハンドルネーム</label>
							<input class="form-control" id="handlename" name="handlename" type="text" value="{$formData.handlename|escape}" pattern="[^\x22\x27\x3c\x3e]*">
							<div class="alert alert-info">
								ハンドルネームはコメント機能を使用時に表示されます。<br>個人を特定できる情報等は入力しないでください。<br>半角の" ' < >の文字は使用できません。
							</div>
						</div>
						<div class="form-group">
							<label>性別 <span class="required">※</span></label>
							<div>
								<label><input type="radio" name="gender" value="male"
									{if $formData.gender=='male'}checked{/if} /> <i class="fa fa-male"></i> 男性</label>&nbsp;&nbsp;
								<label><input type="radio" name="gender" value="female" 
									{if $formData.gender=='female'}checked{/if} /> <i class="fa fa-female"></i> 女性</label>
							</div>
							{if $genderError}
							<div class="alert alert-danger form-check">{$genderError}</div>
							{/if}
						</div>
						<div class="form-group">
							<label for="birthday">生年月日</label>
							<input class="form-control" id="birthday" name="birthday" type="text" value="{$formData.birthday}">
							<div class="alert alert-info">
								入力した誕生日から、該当する "星座" が自動的に指定されます。<br>
								例：2016-02-25
							</div>
						</div>
						<div class="form-group">
							<label for="prefecture">お住まいの都道府県名</label>
							{html_options name="prefecture"
							options=$prefectureOptions
							selected=$formData.prefecture
							class="form-control" }
							<!--
							<div class="alert alert-info">
								今後さらに、より良いサイトにするための参考にさせて頂きます。ご協力お願い致します。<br>
							</div>
							-->
						</div>

						<br/>
						<hr>

						<h2 class="fcolor_blue tecen"><i class="fa fa-rss"></i> メール受信設定</h2>

						<span style="color: #333333;">
							占いランキングの集計結果をメールでお知らせいたします。<br/>
							お送りする時間と曜日、また休日も送ってよいか指定して下さい。<br/>
							なお、配信する時刻は、おおよその目安です。多少遅れる場合もありますので、ご了承ください。<br/>
							配信メールは、sender@uranairanking.jp から送信します。<br/>
							迷惑メール防止のため、メールの受信設定をしている場合は、uranairanking.jp をドメイン指定解除してください｡<br/>
						</span>
						<br/>

						<div class="form-group">
						<!--
						<label>集計結果メールを受信しますか。</label>
						<div>
							{html_options name="notifyMailSw"
								options=$notifyMailSwOptions
								selected=$formData.notificationSw
								class="form-control" }
						</div>

						<hr>
						-->
						<input type="hidden" name="notifyMailSw" value="1">

						<label>受信希望時刻を指定して下さい。</label>
						<div>
							{html_options name="notificationHour"
							options=$notificationHourOptions
							selected=$formData.notificationHour
							class="form-control" }
						</div>

						<hr>

						<label>受信する曜日を指定して下さい。<span class="required">※</span></label>
						<div>
							<label class="touch"><input name="notification[monday]" type="checkbox" value="1"
								{if $formData.notification.monday}checked{/if} /> 月曜日</label>
							<label class="touch"><input name="notification[tuesday]" type="checkbox" value="1"
								{if $formData.notification.tuesday}checked{/if} /> 火曜日</label>
							<label class="touch"><input name="notification[wednesday]" type="checkbox" value="1"
								{if $formData.notification.wednesday}checked{/if} /> 水曜日</label>
							<label class="touch"><input name="notification[thursday]" type="checkbox" value="1"
								{if $formData.notification.thursday}checked{/if} /> 木曜日</label>
							<label class="touch"><input name="notification[friday]" type="checkbox" value="1"
								{if $formData.notification.friday}checked{/if} /> 金曜日</label>
							<label class="touch"><input name="notification[saturday]" type="checkbox" value="1"
								{if $formData.notification.saturday}checked{/if} /> 土曜日</label>
							<label class="touch"><input name="notification[sunday]" type="checkbox" value="1"
								{if $formData.notification.sunday}checked{/if} /> 日曜日</label>
						</div>
						{if $notificationDaysError}
						<div class="alert alert-danger form-check">{$notificationDaysError}</div>
						{/if}

						<hr>

						<label>
							祝日も受信しますか。<span class="required">※</span>
						</label>
						<div>
							<label class="touch"><input name="notificationHolidays" type="radio" value="NO"
								{if $formData.notificationHolidays=='NO'}checked{/if}> いいえ</label>
							<label class="touch"><input name="notificationHolidays" type="radio" value="YES"
								{if $formData.notificationHolidays=='YES'}checked{/if}> はい</label>
						</div>

						{if $notificationHolidaysError}
						<div class="alert alert-danger form-check">{$notificationHolidaysError}</div>
						{/if}

						<hr>

						<label>
							コメント機能の配信設定
						</label>
						<div class="note">※コメント機能とは、取得しているサイトの口コミを書ける機能です。<a href='{sitelink mode="howtouse/comment"}'>詳細はこちら</a></div>
						<div>
							<label class="touch"><input name="notificationCommentPublished" type="checkbox" value="YES" {if $formData.notificationCommentPublished=='YES'}checked{/if}> コメントの審査が通過した時、メールを受信</label>
							<label class="touch"><input name="notificationCommentRejected" type="checkbox" value="YES" {if $formData.notificationCommentRejected=='YES'}checked{/if}> コメントの審査が無効になった時、メールを受信</label>
						</div>

					</div>

					<div class="text-center">
						{if $user}
						<input type="submit" class="btn fontw-backb" value="保存">
						{else}
						<input type="submit" class="btn fontw-backb" value="登録">
						{/if}
					</div>
				</form>
			</div>
		</div>
	</div>
</div>

<script>
	formCheckIndicator({$formFields});
	formCheckErrors({$formErrors});
</script>

{/block}