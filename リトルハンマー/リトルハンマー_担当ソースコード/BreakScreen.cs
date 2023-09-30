using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;
using UnityEngine.SceneManagement;

public class BreakScreen : MonoBehaviour
{
    static public BreakScreen instance;
    [SerializeField] private bool useGravity = true;        // �d�͂�L���ɂ��邩�ǂ���
    [SerializeField] private float explodeForce = 50f;      // ������͂̔{��
    private Rigidbody[] rigidbodies;                        // �q�I�u�W�F�N�g(�j��)��Rigidbody�擾�p
    private Transform[] childs;                             // �q�I�u�W�F�N�g(�j��)��Transform�擾�p
    private Vector3[] startPos;                           // �����G�t�F�N�g�Đ��ʒu
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
        // ��ʊO�ɏo���j�Ђ��\���ɂ���
        foreach (Transform child in childs)
        {
            if(child != null)
            {
                if(child.gameObject.transform.position.y < -30.0f)
                {
                    child.gameObject.transform.localPosition = new Vector3(0.0f, 0.0f, 0.0f);        // ���ȏ�j�Ђ������Ȃ��Ȃ�����ʒu�����Z�b�g
                    child.gameObject.SetActive(false);                                               // �����Ȃ���Ԃɂ���
                    child.gameObject.GetComponent<Rigidbody>().velocity = Vector3.zero;
                }
            }
        }
    }

    // �X�N���[���j�󏈗�(UniRX�Œʒm���󂯎���Ď��s)
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

    // �X�N���[���j��
    public void BreakStart()
    {
        // �ʒu���J�����̉�p�Ɏ��܂�悤�Ɉړ�
        GameObject camera = GameObject.Find("Main Camera");
        this.gameObject.transform.position = new Vector3(camera.transform.position.x, camera.transform.position.y, camera.transform.position.z + 18.74f);

        // �q�I�u�W�F�N�g��Rigidbody�T��
        foreach (Rigidbody rb in rigidbodies)
        {
            rb.useGravity = useGravity;                             // �d�͗L����
            rb.mass = Random.Range(2.5f, 4.0f);                     // ���ʂ�j�Ђ��ƂɃ����_���U�蕪��
            rb.isKinematic = false;                                 // �����L��
            var powerx = Random.Range(-100, 100f)* explodeForce;    // X�����ɐ�����ԗ͂������_���U�蕪��
            var powery = Random.Range(0, 20f)* explodeForce;        // Y�����ɐ�����ԗ͂������_���U�蕪��
            rb.AddForce(new Vector3(powerx, powery));               // �͂�������
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
