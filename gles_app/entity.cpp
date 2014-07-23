
void loadTexture(
    char           *path,
    unsigned int   *width,
    unsigned int   *height,
    char          **pixels
    )
{
    ILuint  texid;
//    ILenum  error;
    char *location = "C:/cygwin/home/richard/dev/gles_app/gles_app/player.png";

    ilInit();

    ilGenImages(1, &texid);
    ilBindImage(texid);
    ilLoadImage((const wchar_t *) location);

    *width = ilGetInteger(IL_IMAGE_WIDTH);
    *height = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    *pixels = malloc((*width) * (*height) * 4);

    memcpy(*pixels, (void *) ilGetData(), (*width) * (*height) * 4);
}


static void textureLoad(
    void
    )
{
    // Load texture.
    glGenTextures(1, &textureId);
    if(textureId == 0)
    {
        goto cleanup;
    }

    // -------------------
    //  Setup texture
    // -------------------

    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    loadTexture("test", &width, &height, &pixel_data);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 (GLint) GL_RGBA,
                 (GLsizei) width,
                 (GLsizei) height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 pixel_data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void Entity::Entity()
{

}


void Entity::draw(
    )
{

}
