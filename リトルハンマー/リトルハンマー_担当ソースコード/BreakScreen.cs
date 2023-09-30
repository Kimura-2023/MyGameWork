using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;
using UnityEngine.SceneManagement;

public class BreakScreen : MonoBehaviour
{
    static public BreakScreen instance;
    [SerializeField] private bool useGravity = true;        // 重力を有効にするかどうか
    [SerializeField] private float explodeForce = 50f;      // 加える力の倍率
    private Rigidbody[] rigidbodies;                        // 子オブジェクト(破片)のRigidbody取得用
    private Transform[] childs;                             // 子オブジェクト(破片)のTransform取得用
    private Vector3[] startPos;                           // 割れるエフェクト再生位置
    // Start is called before the first frame update
    void Start()
    {
        this.gameObject.SetActive(false);
        rigidbodies = GetComponentsInChildren<Rigidbody>();
        childs = GetComponentsInChildren<Transform>();

        if(BreakScreenManager.instance != null)
            BreakScreenManager.instance.breakscreen.Where(x => x == true).Subscribe(_ => Break());

        //Break();
    }

    private void FixedUpdate()
    {
        // 画面外に出た破片を非表示にする
        foreach (Transform child in childs)
        {
            if(child != null)
            {
                if(child.gameObject.transform.position.y < -30.0f)
                {
                    child.gameObject.transform.localPosition = new Vector3(0.0f, 0.0f, 0.0f);        // 一定以上破片が見えなくなったら位置をリセット
                    child.gameObject.SetActive(false);                                               // 見えない状態にする
                    child.gameObject.GetComponent<Rigidbody>().velocity = Vector3.zero;
                }
            }
        }
    }

    // スクリーン破壊処理(UniRXで通知を受け取って実行)
    public void Break()
    {
        if (this == null)
            return;
        this.gameObject.SetActive(true);

        foreach (Transform child in childs)
        {
            child.gameObject.SetActive(true);
        }
        StartCoroutine("BreakStart");
        //BreakScreenManager.instance.SetImage(BreakImage.instance.GetImage());
    }

    // スクリーン破壊
    public void BreakStart()
    {
        // 位置をカメラの画角に収まるように移動
        GameObject camera = GameObject.Find("Main Camera");
        this.gameObject.transform.position = new Vector3(camera.transform.position.x, camera.transform.position.y, camera.transform.position.z + 18.74f);

        // 子オブジェクトのRigidbody探索
        foreach (Rigidbody rb in rigidbodies)
        {
            rb.useGravity = useGravity;                             // 重力有効化
            rb.mass = Random.Range(2.5f, 4.0f);                     // 質量を破片ごとにランダム振り分け
            rb.isKinematic = false;                                 // 物理有効
            var powerx = Random.Range(-100, 100f)* explodeForce;    // X方向に吹っ飛ぶ力をランダム振り分け
            var powery = Random.Range(0, 20f)* explodeForce;        // Y方向に吹っ飛ぶ力をランダム振り分け
            rb.AddForce(new Vector3(powerx, powery));               // 力を加える
        }

        foreach (Transform child in childs)
        {
            var posz = Random.Range(0.0f, 1.0f);
            child.gameObject.transform.position = new Vector3(child.gameObject.transform.position.x, child.gameObject.transform.position.y, child.gameObject.transform.position.z + posz);
        }
        BreakScreenManager.instance.ResetBreak();
        //Fade.instance.FadeOut();
    }
}
