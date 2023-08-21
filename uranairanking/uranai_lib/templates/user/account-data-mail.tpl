{$user_data.handlename}さん、

{if $account_new}
12星座占いランキング[{$host}]へご登録いただきありがとうございます。
{else}
12星座占いランキング[{$host}]のユーザー情報を変更いたしました。
{/if}

ご登録ユーザー情報：
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
● メールアドレス： {$user_data.email}
● ハンドルネーム： {$user_data.handlename}
● 性別： {if $user_data.gender=='male'}男性{else}女性{/if} 
● 生年月日：{$user_data.birthday}
● 都道府県：{$user_prefecture}
● メール受信設定:
　　{if $selected_notification=='1'}集計結果メールを受信します。
　　送信時刻：{$user_data.notificationHour} 頃
　　曜日(○印の曜日に配信されます)：
　　[{if $user_data.notification.monday}○{else}×{/if}] 月曜日
　　[{if $user_data.notification.tuesday}○{else}×{/if}] 火曜日
　　[{if $user_data.notification.wednesday}○{else}×{/if}] 水曜日
　　[{if $user_data.notification.thursday}○{else}×{/if}] 木曜日
　　[{if $user_data.notification.friday}○{else}×{/if}] 金曜日
　　[{if $user_data.notification.saturday}○{else}×{/if}] 土曜日
　　[{if $user_data.notification.sunday}○{else}×{/if}] 日曜日
　　祝日：{if $user_data.notificationHolidays=='YES'}受信します。{else}受信しません。{/if} 
{else}集計結果メールを受信しません。
{/if}
● コメント機能の受信設定:
　　[{if $user_data.notificationCommentPublished=='YES'}○{else}×{/if}] コメントの審査が通過した時、メールを受信
　　[{if $user_data.notificationCommentRejected=='YES'}○{else}×{/if}] コメントの審査が無効になった時、メールを受信

ユーザー情報を変更する時は、下記よりログインして下さい：
http://{$host}/account/login
